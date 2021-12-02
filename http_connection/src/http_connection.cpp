#include "http_connection.h"
#include "config.h"

#include <nlohmann/json.hpp>

#include <cpprest/http_client.h>
#include <cpprest/http_listener.h> // HTTP server
#include <cpprest/json.h>          // JSON library

#ifdef _WIN32
#include <boost/locale/encoding.hpp>
#endif

using namespace sec21;

auto to_platform_specific_string(std::string const& value)
{
#ifdef _WIN32
   return boost::locale::conv::utf_to_utf<wchar_t>(value);
#else
   return value;
#endif
}

struct http_connection::impl
{
   web::http::client::http_client client;

   explicit impl(std::string_view host) : client(to_platform_specific_string(std::string{host}).data()) {}

   //! \brief implicit a GET request
   template <typename Callable>
   void get_request(std::string const& endpoint, Callable&& func)
   {
      make_task_request(web::http::methods::GET, endpoint)
         .then([](web::http::http_response response) {
           if (response.status_code() == web::http::status_codes::OK) {
              return response.extract_json();
           }
           return pplx::task_from_result(web::json::value{});
         })
         .then([=](pplx::task<web::json::value> previous) {
           try {
              std::invoke(func, previous.get());
           } catch (web::http::http_exception& e) {
              // spdlog::error("HTTP error: {}", e.what());
           }
         })
         .wait();
   }

   template <typename Callable>
   void post_request(std::string const& endpoint, std::string value, Callable&& func)
   {
      make_task_request(web::http::methods::POST, endpoint, std::move(value))
         .then([](web::http::http_response response) {
            if (response.status_code() == web::http::status_codes::OK) {
               return response.extract_json();
            }
            return pplx::task_from_result(web::json::value{});
         })
         .then([=](pplx::task<web::json::value> previous) {
            try {
               std::invoke(func, previous.get());
            } catch (web::http::http_exception& e) {
               // spdlog::error("HTTP error: {}", e.what());
            }
         })
         .wait();
   }

 private:
   auto make_task_request(web::http::method mtd, std::string const& endpoint) -> pplx::task<web::http::http_response>
   {
      return client.request(mtd, to_platform_specific_string(endpoint));
   }
   auto make_task_request(web::http::method mtd, std::string const& endpoint, std::string value)
   -> pplx::task<web::http::http_response>
   {
      return client.request(mtd, to_platform_specific_string(endpoint), std::move(value));
   }
};

http_connection::http_connection(std::string_view host) : pimpl{std::make_unique<impl>(host)} {}

http_connection::~http_connection() = default;

nlohmann::json http_connection::get(std::string const& endpoint)
{
   nlohmann::json result;
   pimpl->get_request(endpoint,
                       [&result](web::json::value const& v) { result = nlohmann::json::parse(v.serialize()); });
   //! \todo exception border
   return result;
}

nlohmann::json http_connection::post(std::string const& endpoint, nlohmann::json const& value)
{
   nlohmann::json result;

   //! \todo check for error
   pimpl->post_request(endpoint, value.dump(),
                       [&result](web::json::value const& v) { result = nlohmann::json::parse(v.serialize()); });

   //! \todo exception border
   return result;
}

nlohmann::json http_get(std::string const& host, std::string const& endpoint)
{
   http_connection connection{host};
   return connection.get(endpoint);
}