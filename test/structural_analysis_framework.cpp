#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "sec21/structural_analysis/framework.h"

auto example1() 
{
   using namespace sec21::structural_analysis;

   framework_t result;

   auto n1 = add_node(result, { { 0, 0 }, support::HINGED });
   auto n2 = add_node(result, { { 9, 0 }, support::HINGED });
   auto n3 = add_node(result, { { 3, 8 } });

   auto h1 = force{ 18, 0 };
   auto v1 = force{ 0, -12 };

   add_force(result, n3, h1);
   add_force(result, n3, v1);

   auto s1 = add_rod2(result, n1, n3, { "s1" });
   auto s2 = add_rod2(result, n2, n3, { "s2" });

   //Result:
   //   S1 = 8.54
   //   S2 = -25
   //   Av = -8.0
   //   Ah = -3
   //   Bv = 20
   //   Bh = 15

   return result;
}

TEST_CASE("matrix_A", "[structural analysis]")
{
   using namespace sec21::structural_analysis;

   auto ma = matrix_A(example1());

   const std::vector<float> expected = {
         0.35112f, 0, 1, 0, 0, 0,
         0.93632f, 0, 0, 1, 0, 0,
         0, -0.6f, 0, 0, 1, 0,
         0, 0.8f, 0, 0, 0, 1,
         -0.35112f, 0.6f, 0, 0, 0, 0,
         -0.93632f, -0.8f, 0, 0, 0, 0 };

   REQUIRE(std::equal(std::begin(ma), std::end(ma), std::begin(expected), [](auto a, auto b) { return a == Approx(b); }));
}

TEST_CASE("vector_b", "[structural analysis]")
{
   using namespace sec21::structural_analysis;

   auto vb = vector_b(example1());

   const std::vector<float> expected{ 0, 0, 0, 0, 18.0, -12.0 };

   REQUIRE(std::equal(std::begin(vb), std::end(vb), std::begin(expected), [](auto a, auto b) { return a == Approx(b); }));
}

//#include <boost/compute/core.hpp>
#include <Eigen/Core>
//! \todo only undirected graphs
//! \return A * x = b -> return x
template <typename T>
auto solve(std::vector<T> & A, std::vector<T> & b)
{
   //namespace compute = boost::compute;

   using namespace std;
   using matrix_align_t = Eigen::Matrix<T, -1, -1, Eigen::RowMajor>;
   const auto n = static_cast<std::size_t>(std::sqrt(A.size()));
   Eigen::Map<matrix_align_t> A_(A.data(), n, n);

   using vector_t = Eigen::Matrix<T, -1, 1>;
   Eigen::Map<vector_t> b_(b.data(), n);
   
   //Eigen::ColPivHouseholderQR<matrix_align_t> dec(A_);
   //auto x = dec.solve(b_);
   //{
   //   std::ofstream ofs("matrix.txt");
   //   ofs << x << endl;
   //}


   //// get the default device
   //compute::device device = compute::system::default_device();
   //// print the device's name
   //std::cout << "hello from " << device.name() << std::endl;


   int z{ 7 };

   return 5;
}


TEST_CASE("solve", "[structural analysis]")
{
   using namespace sec21::structural_analysis;

   auto f{ example1() };

   auto mA = matrix_A(f);
   auto vb = vector_b(f);
   auto x = solve(mA, vb);



   const std::vector<float> expected{ 8.54f, -25.f, -8.f, -3.f, 2.f, 15.f };

   //REQUIRE(std::equal(std::begin(x), std::end(x), std::begin(expected), [](auto a, auto b) { return a == Approx(b); }));
}

/*

template<
   typename FrameworkType>
bool valid(FrameworkType const& g)
{
   if (!boost::num_vertices(g) || !boost::num_vertices(g))
      return false;

   // if any node is not connected then framework is invalid
#ifdef BOOST_NO_CXX14_GENERIC_LAMBDAS
   typedef typename boost::graph_traits<FrameworkType>::vertex_descriptor vd_type;

   std::for_each(
      boost::vertices(g).first,
      boost::vertices(g).second,
      [&](vd_type const& vd) {
         auto ret = boost::adjacent_vertices(vd, g);

         if (ret.first == ret.second)
            return false;
   });
#else
   std::for_each(
      boost::vertices(g).first,
      boost::vertices(g).second,
      [&](auto const& vd) {
         auto ret = boost::adjacent_vertices(vd, g);

         if (ret.first == ret.second)
            return false;
   });
#endif

   return true;
}
*/
