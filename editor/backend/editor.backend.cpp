#include "config.h"
#include "invoke.h"
#include "response.h"
#include "command_loadfile.h"
#include "command_add_node.h"
#include "command_add_member.h"
#include "command_add_support.h"
// websocket
#include "server.h"
#include "session.h"

#include <sec21/overloaded.h>
#include <sec21/structural_analysis/node.h>
#include <sec21/structural_analysis/space_truss.h>

// logging
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <variant>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <memory>
#include <fstream>
#include <filesystem>

namespace sec21::editor
{
   //! \todo temporary replacement for structural_analysis::member; issues with units and c++17
   template <typename Descriptor, typename Precision = double>
   struct member
   {
      using descriptor_t = Descriptor;
      using precision_t = Precision;

      descriptor_t name{ structural_analysis::descriptor_traits<descriptor_t>::invalid() };
   };

   template <typename Descriptor, typename Precision>
   void to_json(nlohmann::json& j, member<Descriptor, Precision> const& obj) {
      j = nlohmann::json{
         {"name", obj.name}
      };
   }

   template <typename Descriptor, typename Precision>
   void from_json(nlohmann::json const& j, member<Descriptor, Precision>& obj) 
   {
      j.at("name").get_to(obj.name);
   }

   using node_t = structural_analysis::node<2, std::size_t, float>;
   //! \todo replace with structural_analysis::member
   using member_t = member<std::size_t, float>;
   using model_t = structural_analysis::space_truss<node_t, member_t>;


   //! \todo constrain template parameter
   template <typename Descriptor>
   auto increment_desc(Descriptor descriptor) noexcept 
   { 
      return ++descriptor; 
   }

   //! \todo actions playback -> test update model via json files
   template <typename Model, typename... Actions>
   std::string update(Model& model, std::variant<Actions...> action) noexcept
   {
      const auto result = std::visit(
         overloaded{
            [&model](command_loadfile_t const& cmd) 
            { 
               spdlog::debug("command: load file: {}", cmd.argument);
               std::ifstream ifs{ std::filesystem::current_path() / cmd.argument };
               const auto j = nlohmann::json::parse(ifs);
               model = j;
               return response{ ResponseType::model, j.dump() };
            },
            [&model](command_add_node_t const& cmd) 
            {
               spdlog::debug("command: add node");

               // bool b1 = std::any_of(std::begin(model.nodes), std::end(model.nodes), [&obj](auto const& n){ return n.position == obj.position; });
               // if (b1) {
               //    spdlog::debug("found node at position");
               //    return response{ ResponseType::error, "node already exists at position: TODO" };
               // }

               node_t obj{};
               //! \todo write a generic function
               obj.name = increment_desc(std::size(model.nodes));
               obj.position = to_array(cmd.argument);

               if (!structural_analysis::add_node(model, obj))
                  return response{ ResponseType::error, "failed to add node" };  //! \todo error description

               nlohmann::json j;
               j = model;
               return response{ ResponseType::model, j.dump() };
            },
            [&model](command_add_member_t const& cmd)
            {
               spdlog::debug("command: add member");
               try 
               {
                  member_t obj;
                  obj.name = increment_desc(std::size(model.members));

                  const auto f = model.nodes.at(cmd.argument.from);
                  const auto t = model.nodes.at(cmd.argument.to);

                  if (!structural_analysis::add_member(model, f.name, t.name, obj))
                     return response{ ResponseType::error, "failed to add member" };  //! \todo error description
               }
               catch (std::exception& ex)
               {
                  return response{ ResponseType::error, ex.what() };
               }
               catch (...)
               {
                  return response{ ResponseType::error, "unhandled exception" };
               }
               nlohmann::json j;
               j = model;
               return response{ ResponseType::model, j.dump() };
            },
            [&model](command_add_support_t const& cmd)
            {
               spdlog::debug("command: add support");
               try 
               {
                  auto& n = model.nodes.at(cmd.argument.node);
                  n.global_support = node_t::global_support_t{cmd.argument.px, cmd.argument.py};
               }
               catch (std::exception& ex)
               {
                  return response{ ResponseType::error, ex.what() };
               }
               catch (...)
               {
                  return response{ ResponseType::error, "unhandled exception" };
               }
               nlohmann::json j;
               j = model;
               return response{ ResponseType::model, j.dump() };
            }
         },	
         action
      );
      const auto j = nlohmann::json(result);
      // transform to std::string
      return j.dump();
   }

   template <typename... Commmands>
   class controller
   {
      model_t model{};

      using actions_t = std::variant<Commmands...>;
      // std::vector<event_t> input_events{}; 

   public:
      auto operator()(std::string_view raw_input)
      {
         const auto input_processor = input_dispatcher<std::string, Commmands...>([&](auto action) {
            if (action) {
               return update(this->model, *action);
            }
            return std::string{ "invalid input" };
         });
         return std::invoke(input_processor, raw_input);
      }
   };

   template <typename... Commmands>
   class playback_controller
   {
      model_t model{};

      using command_t = std::variant<Commmands...>;
      std::vector<command_t> commands{}; 

   public:
      explicit playback_controller(std::vector<command_t> command_list) noexcept
         : commands{ std::move(command_list) } 
      {}

      auto operator()(std::string_view)
      {
         std::for_each(begin(commands), end(commands), [this](auto cmd){ update(this->model, cmd); });
      }
   };   
}


//! \todo handle and log sigterm
int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{
	try
	{
		spdlog::set_level(spdlog::level::debug);
		spdlog::info("starting application sec21::editor.backend {}.{}", VERSION_MAJOR, VERSION_MINOR);

		//! \todo config extern
		//! \todo net::ip::make_address(argAddr);
		const auto address = boost::asio::ip::tcp::v4();
		const std::uint16_t port = 7654;

		spdlog::info("listen on port: {}", port);

      using namespace sec21;
      using controller_t = editor::controller<
         editor::command_loadfile_t, 
         editor::command_add_node_t, 
         editor::command_add_member_t,
         editor::command_add_support_t
      >;
      using session_t = websocket::session<controller_t>;

#if 0
		websocket::server<session_t> srv({address, port});
		const auto exit_code = std::invoke(srv);
		spdlog::info("exit code {}", exit_code);
#endif

	}
	catch (std::exception &ex)
	{
		spdlog::critical("exception {}", ex.what());
		return -1;
	}
	return 0;
}
