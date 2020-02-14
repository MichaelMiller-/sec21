#pragma once

#if 0
#include <limits>

#include <sec21/strong_type.h>
#include <sec21/policies/comparable.h>
#include <sec21/policies/printable.h>
#include <sec21/policies/sortable.h>
#include <sec21/structural_analysis/support.h>

#include <boost/qvm/vec.hpp>

 #include <boost/geometry/algorithms/distance.hpp> 

 // boost::geometry::distance(p, poly)

namespace sec21::structural_analysis
{
   //! \todo policy "printable" is only used for Unittests.
   using node_id_t = strong_type<size_t, struct node_id_tag, comparable, printable>;
   using member_id_t = strong_type<size_t, struct member_id_tag, comparable, sortable, printable>;

   // constants
   constexpr const auto invalid_node_id{ std::numeric_limits<node_id_t::type>::max() };
   constexpr const auto invalid_member_id{ std::numeric_limits<member_id_t::type>::max() };

   //! \brief Container class which bundles ID with Datastructure
   template <typename ID, typename Properties>
   struct item
   {
      using descriptor = ID;

   private:
      descriptor  m_id{ std::numeric_limits<ID>::max() };
      Properties  m_data{};

      template<typename, typename> friend class system;

   public:
      template <typename... Args>
      explicit item(typename descriptor::type id, Args &&... args) 
         : m_id{ id }
         , m_data{ std::forward<Args>(args)... }
      {}

      constexpr auto id() const noexcept { return m_id; }
      constexpr auto data() const noexcept { return m_data; }

      constexpr bool operator == (item const& other) const noexcept {
         return m_id == other.m_id;
      }
      constexpr bool operator != (item const& other) const noexcept { 
         return not (*this == other); 
      }

      void swap(item& other) noexcept
      {
         using std::swap;
         std::swap(m_id, other.m_id);
         std::swap(m_data, other.m_data);
      }
   };

   //! \todo possible 
   // template <typename NodeProperties, typename MemberProperties, typename Tag, template<typename> typename... Policies>
   // struct system : public Policies<system<NodeProperties, MemberProperties, Tag, Policies...>>...
   // { }

   template <typename NodeProperties, typename MemberProperties>
   struct system_data
   {
      using node_t = item<node_id_t, NodeProperties>;
      using member_t = item<member_id_t, MemberProperties>;

      std::vector<node_t>     nodes{};
      std::vector<member_t>   members{};
   };

   //! \todo is not a good name; name mismatch with cstdlib.h system() (Stabwerkssystem)
   //! \todo sollte da system auch die informationen über die einheiten halten?
   template <typename NodeProperties, typename MemberProperties>
   class system
   {
   // private:
   public:
      using node_t = item<node_id_t, NodeProperties>;
      using member_t = item<member_id_t, MemberProperties>;

      using node_descriptor = typename node_t::descriptor;
      using member_descriptor = typename member_t::descriptor;

      std::vector<node_t>     m_nodes{};
      std::vector<member_t>   m_members{};

   private:
      using diff_t = typename decltype(m_nodes)::difference_type;
      // member id -> start and end node
      std::map<member_descriptor, std::pair<diff_t, diff_t>>  m_adjacency_list{};

   public:
      //! \todo noexcept ?
      auto add_node(NodeProperties && node) /*noexcept*/ -> node_descriptor
      {
         const auto result = m_nodes.emplace_back(num_nodes(), std::forward<NodeProperties>(node));
         return node_descriptor{ result.m_id };
      }

      //! \todo noexcept ?
      template <typename... Args>
      auto add_node(Args &&... args) -> node_descriptor
      {
         const auto result = m_nodes.emplace_back(num_nodes(), std::forward<Args>(args)...);
         return node_descriptor{ result.m_id };
      }

      //! \todo noexcept ?
      template <typename... Args>
      auto add_member(node_descriptor from, node_descriptor to, Args &&... args) -> member_descriptor
      {
         const auto it_from = find(from);
         const auto it_to = find(to);

         //! \todo exception?
         if (it_from == std::end(m_nodes) || it_to == std::end(m_nodes))
            return member_descriptor{ invalid_member_id };

         const auto result = m_members.emplace_back(num_members(), std::forward<Args>(args)...);
         const auto id = member_descriptor{ result.m_id };

         m_adjacency_list[id] = std::make_pair(std::distance(std::begin(m_nodes), it_from), std::distance(std::begin(m_nodes), it_to));

         return id;
      }

      void erase(member_descriptor id)
      {
         const auto it = find(id);

         if (it == std::end(m_members))
            //! \todo own exception class / struct
            throw std::invalid_argument("unkown member id");

         m_members.erase(it);
         m_adjacency_list.erase(id);
      }

      //! \todo public? really?
      auto nodes() const noexcept { return m_nodes; }

      auto num_nodes() const noexcept { return m_nodes.size(); }
      auto num_members() const noexcept { return m_members.size(); }

      //! \todo erase node


      //auto length(member_descriptor id)
      //{
      //   const auto nodes = m_adjacency_list.at(id);
      //}


   private:
      //! \todo public?

      void clear() noexcept 
      { 
         m_nodes.clear(); 
         m_members.clear();
      }

   private:
      template <typename Container, typename Descriptor>
      auto find(Container && c, Descriptor id) noexcept {
         return std::find_if(std::begin(c), std::end(c), [&id](auto && e) { return id == e.m_id; });
      }

      auto find(node_descriptor id) noexcept { return find(m_nodes, id); }
      auto find(member_descriptor id) noexcept { return find(m_members, id); }
   };


   //! \todo
   //template <typename System>
   //struct system_traits
   //{
   //   using node_descriptor = typename System::node_descriptor;
   //};

   namespace detail 
   {
      struct node_dimension_tag {};

      template <std::size_t N>
      struct base_node_dimension : node_dimension_tag 
      {
         static_assert(N < 3, "Dimension cannot be greater than 3");

         constexpr static auto value{N};
      };
      template <std::size_t N>
      using node_dimension_t = base_node_dimension<N>;
   }


   template <typename T>
   struct is_node_dimension : std::is_base_of<detail::node_dimension_tag, T>::type {};

   //! \todo rename tags
   //! \todo usw boost::mpl::int_c<1...3>;
   using node_dim_1D = detail::node_dimension_t<1>;
   using node_dim_2D = detail::node_dimension_t<2>;
   using node_dim_3D = detail::node_dimension_t<3>;


   template <size_t N>
   using position_t = boost::qvm::vec<double, N>;

   //! \todo json adapter; e.g. LOHMAN
   //! \todo concept
   //! \todo maybe redisgn
   template <typename Dimension>
   struct node_data
   {
      static_assert(is_node_dimension<Dimension>::value, "ffffffoooooooooooooooooooooooo");

      constexpr static auto dimension_v = Dimension::value;

      //! \brief World Position
      position_t<dimension_v>    position;
      //! \todo should every node has a support, even it is support_t::Free; is this the right way?
      //! \brief
      support_kind               support{ support_kind::Free };
   };
}
#endif
