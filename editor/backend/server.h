#pragma once

#include <spdlog/spdlog.h>

#include <boost/asio/ip/tcp.hpp>

#include <memory>
#include <functional>

namespace sec21::websocket
{
   template <typename Session>
   //! \todo constrain Session: ctor has to accept a socket
   //! \todo constrain Session: has to be callable 
   class server
   {
      boost::asio::io_context context;
      boost::asio::ip::tcp::acceptor acceptor;

   public:
      explicit server(boost::asio::ip::tcp::endpoint endpoint)
         : context{ 1 }
         , acceptor{ context, endpoint }
      {}

      auto operator()()
      {
         accept();
         context.run();
         return 0;
      }

   private:
      void accept()
      {
         acceptor.async_accept([this](auto error, auto socket) 
         {
            if (error)
            {
               spdlog::error("server::accept error: {}", error.message());
               return;
            }

            auto current_session = std::make_shared<Session>(std::move(socket));
            std::invoke(*current_session);

            accept();
         });
      }
   };
}