#pragma once

#include <spdlog/spdlog.h>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <memory>
#include <functional>
#include <deque>

namespace sec21::websocket
{
   //! \todo find a better solution, instead of polute the namespace with "globals"
   std::atomic_uint32_t sid = 0;

   template <typename ResponseHandler>
   class session : public std::enable_shared_from_this<session<ResponseHandler>>
   {
      // fix dependent base lookup. see c++ 14.6.2
      using std::enable_shared_from_this<session<ResponseHandler>>::shared_from_this;

      const std::uint32_t id;
      boost::beast::websocket::stream<boost::asio::ip::tcp::socket> websocket;
      // io
      boost::beast::flat_buffer read_buffer{};
      boost::beast::flat_buffer write_buffer{};

      //! \todo implement as std::variant<std::string, std::vector<uint8_t>, ...> packet
      struct packet
      {
         std::string text_buffer{};
         //! \todo binary_buffer is not required yet
         // std::vector<uint8_t> binary_buffer{};
      };

      std::deque<std::shared_ptr<packet>> write_queue{};

      ResponseHandler response_handler{};

   public:
      explicit session(boost::asio::ip::tcp::socket&& socket)
         : id{ ++sid }
         , websocket{ std::move(socket) }
      {
         // set suggested timeout settings for the websocket
         websocket.set_option(boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::server));
         // set a decorator to change the Server of the handshake
         websocket.set_option(boost::beast::websocket::stream_base::decorator([](boost::beast::websocket::response_type& res)
         {
            const auto str = fmt::format("sec21::editor.backend-{}.{}", VERSION_MAJOR, VERSION_MINOR);
            res.set(boost::beast::http::field::server, str);
         }));
         spdlog::info("session {} created", id);
      }

      auto operator()()
      {
         websocket.async_accept(boost::beast::bind_front_handler(&session::accept, shared_from_this()));
      }

   private:
      void accept(boost::beast::error_code error)
      {
         spdlog::debug("session {} accept()", id);

         if (error)
            return fail(error, "accept");

         read();
      }

      void read()
      {
         websocket.async_read(read_buffer, boost::beast::bind_front_handler(&session::on_read, shared_from_this()));
      }

      void on_read(boost::beast::error_code e, size_t /*length*/)
      {
         if (e == boost::beast::websocket::error::closed) 
            return close();

         if (e) 
            return fail(e, "read");

         if (!websocket.got_text())
         {
            spdlog::warn("Got empty text. Ignoring.");
         }
         else
         {
            auto data = std::make_shared<packet>();
            // packet->text = true;
            const auto raw_data = reinterpret_cast<const char*>(read_buffer.cdata().data());
            data->text_buffer.assign(raw_data, raw_data + read_buffer.size());
            receive(data);
         }

         read_buffer.clear();
         read();
      }
      
      void receive(std::shared_ptr<packet> const& received)
      {
         spdlog::debug("received: \"{}\"", received->text_buffer);
         auto response = std::make_shared<packet>(std::invoke(response_handler, received->text_buffer));
         send(response);
      }

      void send(std::shared_ptr<packet> const& data)
      {
         write_queue.emplace_back(data);

         if (write_queue.size() > 1) // we're already writing
            return;

         write();
      }

      void write()
      {
         assert(!write_queue.empty());

         const auto& data = write_queue.front();
         // websocket.text(data->text);
         websocket.text(true);

         auto handler = boost::beast::bind_front_handler(&session::on_write, shared_from_this());

         // if (data->text) 
            websocket.async_write(boost::asio::buffer(data->text_buffer), std::move(handler));
         // else 
         //    websocket.async_write(boost::asio::buffer(data->binaryBuffer), std::move(handler));
      }

      void on_write(boost::beast::error_code e, std::size_t n)
      {
         spdlog::debug("write {} bytes", n); 
         if (e) 
            return fail(e, "write");

         write_queue.pop_front();
         if (write_queue.empty()) 
            return;

         write();
      }

      void fail(boost::beast::error_code e, std::string_view source) const
      {
         spdlog::error("session {} failed with error {} in {}", id, e.message(), source);
      }

      void close() const
      {
         spdlog::info("session {} closed", id);
      }
   };
}
