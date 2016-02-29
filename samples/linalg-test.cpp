#include "../linalg.h"
using namespace linalg::aliases;

#define CATCH_CONFIG_MAIN
#include "thirdparty/catch.hpp"

template<class T, int M> void require_zero(const linalg::vec<T,M> & v) { for(int i=0; i<M; ++i) REQUIRE( v[i] == 0 ); }
template<class T, int M, int N> void require_zero(const linalg::mat<T,M,N> & m) { for(int j=0; j<N; ++j) require_zero(m[j]); }

TEST_CASE( "linalg types default construct to zero" ) 
{
    require_zero( uint2() );
    require_zero( int3x4() );
    require_zero( ushort4() );
    require_zero( float4x2() );
    require_zero( double2x3() );
}

TEST_CASE( "equality and inequality operators" )
{
    // Two vectors compare equal if all elements are equal
    REQUIRE(   float4(1,2,3,4) == float4(1,2,3,4)  );
    REQUIRE( !(float4(1,2,3,4) == float4(5,2,3,4)) );
    REQUIRE( !(float4(1,2,3,4) == float4(1,5,3,4)) );
    REQUIRE( !(float4(1,2,3,4) == float4(1,2,5,4)) );
    REQUIRE( !(float4(1,2,3,4) == float4(1,2,3,5)) );

    // Two vectors compare inequal if at least one element is inequal
    REQUIRE( !(float4(1,2,3,4) != float4(1,2,3,4)) );
    REQUIRE(   float4(1,2,3,4) != float4(5,2,3,4)  );
    REQUIRE(   float4(1,2,3,4) != float4(1,5,3,4)  );
    REQUIRE(   float4(1,2,3,4) != float4(1,2,5,4)  );
    REQUIRE(   float4(1,2,3,4) != float4(1,2,3,5)  );
}

TEST_CASE( "operator overloads produce correct results" )
{
    // All operators can be applied to vector types, and results are computed elementwise
    REQUIRE( float2(2,3) + float2(4,12) == float2(  6,  15) );
    REQUIRE( float2(2,3) - float2(4,12) == float2( -2,  -9) );
    REQUIRE( float2(2,3) * float2(4,12) == float2(  8,  36) );
    REQUIRE( float2(2,3) / float2(4,12) == float2(0.5,0.25) );
    REQUIRE(  int2(27,31) % int2(5,8)  == int2( 2,  7) );
    REQUIRE( (int2(27,31) | int2(5,8)) == int2(31, 31) );
    REQUIRE( (int2(27,31) ^ int2(5,8)) == int2(30, 23) );
    REQUIRE( (int2(27,31) & int2(5,8)) == int2( 1,  8) );
    REQUIRE(  int2(14,35) << int2(2,3) == int2(56,280) );
    REQUIRE(  int2(14,35) >> int2(2,3) == int2( 3,  4) );
}

TEST_CASE( "integer promotion rules apply" )
{
    // uint8_t promotes to int
    REQUIRE(+byte2()             == int2());
    REQUIRE(-byte3()             == int3());
    REQUIRE(~byte4()             == int4(~0));
    REQUIRE(!byte2()             == bool2(true)); // operator! always returns a vector of bools
    REQUIRE((byte2() +  byte2()) == int2());
    REQUIRE((byte3() -  byte3()) == int3());
    REQUIRE((byte4() *  byte4()) == int4());
    REQUIRE((byte2() /  byte2(1)) == int2());
    REQUIRE((byte3() %  byte3(1)) == int3());
    REQUIRE((byte4() |  byte4()) == int4());
    REQUIRE((byte2() ^  byte2()) == int2());
    REQUIRE((byte3() &  byte3()) == int3());
    REQUIRE((byte4() << byte4()) == int4());
    REQUIRE((byte2() >> byte2()) == int2());

    // int16_t promotes to int
    REQUIRE(+short3()              == int3());
    REQUIRE(-short4()              == int4());
    REQUIRE(~short2()              == int2(~0));
    REQUIRE(!short3()              == bool3(true)); // operator! always returns a vector of bools
    REQUIRE((short3() +  short3()) == int3());
    REQUIRE((short4() -  short4()) == int4());
    REQUIRE((short2() *  short2()) == int2());
    REQUIRE((short3() /  short3(1)) == int3());
    REQUIRE((short4() %  short4(1)) == int4());
    REQUIRE((short2() |  short2()) == int2());
    REQUIRE((short3() ^  short3()) == int3());
    REQUIRE((short4() &  short4()) == int4());
    REQUIRE((short2() << short2()) == int2());
    REQUIRE((short3() >> short3()) == int3());

    // uint16_t promotes to int
    REQUIRE(+ushort4()               == int4());
    REQUIRE(-ushort2()               == int2());
    REQUIRE(~ushort3()               == int3(~0));
    REQUIRE(!ushort4()               == bool4(true)); // operator! always returns a vector of bools
    REQUIRE((ushort4() +  ushort4()) == int4());
    REQUIRE((ushort2() -  ushort2()) == int2());
    REQUIRE((ushort3() *  ushort3()) == int3());
    REQUIRE((ushort4() /  ushort4(1)) == int4());
    REQUIRE((ushort2() %  ushort2(1)) == int2());
    REQUIRE((ushort3() |  ushort3()) == int3());
    REQUIRE((ushort4() ^  ushort4()) == int4());
    REQUIRE((ushort2() &  ushort2()) == int2());
    REQUIRE((ushort3() << ushort3()) == int3());
    REQUIRE((ushort4() >> ushort4()) == int4());

    // int is not promoted
    REQUIRE(+int2()            == int2());
    REQUIRE(-int3()            == int3());
    REQUIRE(~int4()            == int4(~0));
    REQUIRE(!int2()            == bool2(true)); // operator! always returns a vector of bools
    REQUIRE((int2() +  int2()) == int2());
    REQUIRE((int3() -  int3()) == int3());
    REQUIRE((int4() *  int4()) == int4());
    REQUIRE((int2() /  int2(1)) == int2());
    REQUIRE((int3() %  int3(1)) == int3());
    REQUIRE((int4() |  int4()) == int4());
    REQUIRE((int2() ^  int2()) == int2());
    REQUIRE((int3() &  int3()) == int3());
    REQUIRE((int4() << int4()) == int4());
    REQUIRE((int2() >> int2()) == int2());

    // unsigned is not promoted
    REQUIRE(+uint3()             == uint3());
    REQUIRE(-uint4()             == uint4()); // NOTE: Will produce a warning about unary minus applied to unsigned type, this is probably what we want
    REQUIRE(~uint2()             == uint2(~0));
    REQUIRE(!uint3()             == bool3(true)); // operator! always returns a vector of bools
    REQUIRE((uint3() +  uint3()) == uint3());
    REQUIRE((uint4() -  uint4()) == uint4());
    REQUIRE((uint2() *  uint2()) == uint2());
    REQUIRE((uint3() /  uint3(1)) == uint3());
    REQUIRE((uint4() %  uint4(1)) == uint4());
    REQUIRE((uint2() |  uint2()) == uint2());
    REQUIRE((uint3() ^  uint3()) == uint3());
    REQUIRE((uint4() &  uint4()) == uint4());
    REQUIRE((uint2() << uint2()) == uint2());
    REQUIRE((uint3() >> uint3()) == uint3());

    // float is not promoted
    REQUIRE(+float4()              == float4());
    REQUIRE(-float2()              == float2());
    REQUIRE(!float4()              == bool4(true)); // operator! always returns a vector of bools
    REQUIRE((float4() + float4()) == float4());
    REQUIRE((float2() - float2()) == float2());
    REQUIRE((float3() * float3(1)) == float3());
    REQUIRE((float4() / float4(1)) == float4());
}

TEST_CASE( "elementwise comparison functions produce correct results" )
{
    REQUIRE( equal  (float3(1,2,3), float3(4,-2,3)) == bool3(false, false, true ) );
    REQUIRE( nequal (float3(1,2,3), float3(4,-2,3)) == bool3(true,  true,  false) );
    REQUIRE( less   (float3(1,2,3), float3(4,-2,3)) == bool3(true,  false, false) );
    REQUIRE( greater(float3(1,2,3), float3(4,-2,3)) == bool3(false, true,  false) );
    REQUIRE( lequal (float3(1,2,3), float3(4,-2,3)) == bool3(true,  false, true ) );
    REQUIRE( gequal (float3(1,2,3), float3(4,-2,3)) == bool3(false, true,  true ) );
}

#include <vector>
#include <type_traits>

template<class T> void take(const T &) {}
#define MATCH(TYPE, ...) static_assert(std::is_same<TYPE, decltype(__VA_ARGS__)>::value, #TYPE " != " #__VA_ARGS__); take(__VA_ARGS__)

TEST_CASE( "templates instantiate correctly", "" ) 
{
    // Declare some variables to test functions requiring an lvalue
    const float2 cf2; const float3 cf3; const float4 cf4;
    float2 f2; float3 f3; float4 f4; int2 i2; int3 i3; int4 i4;
    float fs[] = {1,2,3,4};

    // Exercise vec<T,2>
    MATCH(float2, float2());
    MATCH(float2, float2(1,2) );
    MATCH(float2, float2(5) );
    MATCH(float2, float2(fs) );
    MATCH(float2, float2(int2(3,4)) );
    MATCH(const float&, cf2[1] );
    MATCH(float&, f2[1] );

    // Exercise vec<T,3>
    MATCH(float3, float3() );
    MATCH(float3, float3(1,2,3) );
    MATCH(float3, float3(float2(),4) );
    MATCH(float3, float3(5) );
    MATCH(float3, float3(fs) );
    MATCH(float3, float3(int3(3,4,5)) );
    MATCH(const float&, cf3[1] );
    MATCH(float&, f3[1] );
    MATCH(float2, float3().xy() );

    // Exercise vec<T,4>
    MATCH(float4, float4() );
    MATCH(float4, float4(1,2,3,4) );
    MATCH(float4, float4(float3(),4) );
    MATCH(float4, float4(5) );
    MATCH(float4, float4(fs) );
    MATCH(float4, float4(int4(3,4,5,6)) );
    MATCH(const float&, cf4[1] );
    MATCH(float&, f4[1] );
    MATCH(float3, float4().xyz() );

    // TODO: Exercise mat<T,M,N> for N=2,3,4

    // Exercise sequence functions
    for(float & f : f4) take(f);
    for(float f : float4()) take(f);
    for(float4 & f : float4x4()) take(f);

    // Exercise relational operators
    MATCH(bool, int2() == int2() );
    MATCH(bool, float3() == float3() );
    MATCH(bool, double4() == double4() );
    MATCH(bool, short2() != short2() );
    MATCH(bool, int2() < int2() );
    MATCH(bool, float3() < float3() );
    MATCH(bool, double4() < double4() );
    MATCH(bool, int2() > int2() );
    MATCH(bool, float3() <= float3() );
    MATCH(bool, double4() >= double4() );
    
    // Exercise unary operators and functions
    MATCH(float3 , +float3() );
    MATCH(float2 , -float2() );
    MATCH(int4   , ~int4() );
    MATCH(bool2  , !short2() );
    MATCH(float3 , abs  (float3()) );
    MATCH(float4 , floor(float4()) );
    MATCH(float3 , ceil (float3()) );
    MATCH(float2 , exp  (float2()) );
    MATCH(float4 , log  (float4()) );
    MATCH(float2 , log10(float2()) );
    MATCH(float3 , sqrt (float3()) );
    MATCH(double4, sin  (double4()) );
    MATCH(double3, cos  (double3()) );
    MATCH(double4, tan  (double4()) );
    MATCH(double3, asin (double3()) );
    MATCH(double2, acos (double2()) );
    MATCH(double4, atan (double4()) );
    MATCH(double2, sinh (double2()) );
    MATCH(double3, cosh (double3()) );
    MATCH(double4, tanh (double4()) );
    MATCH(double4, round(double4()) );

    // Exercise binary operators
    MATCH(float2 , float2 () +  float2 () );
    MATCH(float3 , float3 () -  float3 () );
    MATCH(double4, double4() *  double4() );
    MATCH(double2, double2() /  double2() );
    MATCH(int3   , int3   () %  int3   (1) );
    MATCH(int4   , int4   () |  int4   () );
    MATCH(int2   , short2 () ^  short2 () );
    MATCH(int3   , short3 () &  short3 () );
    MATCH(int3   , int3   () << int3   () );
    MATCH(int4   , int4   () >> int4   () );

    MATCH(float2 , float2 () +  float   () );
    MATCH(float3 , float3 () -  float   () );
    MATCH(double4, double4() *  double  () );
    MATCH(double2, double2() /  double  () );
    MATCH(int3   , int3   () %  int     (1) );
    MATCH(int4   , int4   () |  int     () );
    MATCH(int2   , short2 () ^  short   () );
    MATCH(int3   , short3 () &  short   () );
    MATCH(int3   , int3   () << int     () );
    MATCH(uint4  , uint4  () >> unsigned() );

    MATCH(float2 , float   () +  float2 () );
    MATCH(float3 , float   () -  float3 () );
    MATCH(double4, double  () *  double4() );
    MATCH(double2, double  () /  double2() );
    MATCH(int3   , int     () %  int3   (1) );
    MATCH(int4   , int     () |  int4   () );
    MATCH(int2   , short   () ^  short2 () );
    MATCH(int3   , short   () &  short3 () );
    MATCH(int3   , int     () << int3   () );
    MATCH(uint4  , unsigned() >> uint4  () );

    MATCH(float2&, f2 += float2() );
    MATCH(float3&, f3 -= float3() );
    MATCH(float4&, f4 *= float4() );
    MATCH(float2&, f2 /= float2() );
    MATCH(int2&  , i2 %= int2(1) );
    MATCH(int3&  , i3 |= int3() );
    MATCH(int4&  , i4 ^= int4() );
    MATCH(int2&  , i2 &= int2() );
    MATCH(int3&  , i3 <<= int3() );
    MATCH(int4&  , i4 >>= int4() );

    MATCH(float2&, f2 += float() );
    MATCH(float3&, f3 -= float() );
    MATCH(float4&, f4 *= float() );
    MATCH(float2&, f2 /= float() );
    MATCH(int2&  , i2 %= int(1) );
    MATCH(int3&  , i3 |= int() );
    MATCH(int4&  , i4 ^= int() );
    MATCH(int2&  , i2 &= int() );
    MATCH(int3&  , i3 <<= int() );
    MATCH(int4&  , i4 >>= int() );

    MATCH(float2 , min  (float2(), float2()) );
    MATCH(float3 , max  (float3(), float3()) );
    MATCH(float2 , fmod (float2(), float2()) );
    MATCH(float3 , pow  (float3(), float3()) );
    MATCH(float4 , atan2(float4(), float4()) );
    MATCH(float3 , clamp(float3(), float3(), float3()) );

    // Exercise componentwise relational ops
    MATCH(bool2, equal  (float2(), float2()) );
    MATCH(bool3, nequal (float3(), float3()) );
    MATCH(bool4, less   (double4(), double4()) );
    MATCH(bool2, greater(double2(), double2()) );
    MATCH(bool3, lequal (int3(), int3()) );
    MATCH(bool4, gequal (int4(), int4()) );

    // Exercise reduction functions
    MATCH(bool, any(bool3()) );
    MATCH(bool, all(bool4()) );
    MATCH(int, sum(int2()) );
    MATCH(float, product(float4()) );

    // Exercise selection functions
    MATCH(int, argmin(float2()) );
    MATCH(int, argmax(float3()) );
    MATCH(float, minelem(float4()) );
    MATCH(float, maxelem(float2()) );

    // Exercise vector algebra functions
    MATCH(float, cross(float2(), float2()) );
    MATCH(float3, cross(float3(), float3()) );
    MATCH(float, dot(float4(), float4()) );
    MATCH(float, length2(float2()) );
    MATCH(float, length(float3()) );
    MATCH(float, distance2(float4(), float4()) );
    MATCH(float, distance(float2(), float2()) );
    MATCH(float3, normalize(float3()) );
    MATCH(float4, lerp(float4(), float4(), float()) );
    MATCH(float4, nlerp(float4(), float4(), float()) );
    MATCH(float4, slerp(float4(), float4(), float()) );

    // Exercise quaternion algebra functions
    MATCH(float4, qconj(float4()) );
    MATCH(float4, qinv(float4()) );
    MATCH(float4, qmul(float4(), float4()) );
    MATCH(float3, qxdir(float4()) );
    MATCH(float3, qydir(float4()) );
    MATCH(float3, qzdir(float4()) );
    MATCH(float3, qrot(float4(), float3()) );
    MATCH(float , qangle(float4()) );
    MATCH(float3, qaxis(float4()) );
    MATCH(float4, qnlerp(float4(), float4(), float()) );
    MATCH(float4, qslerp(float4(), float4(), float()) );

    // TODO: mul, adjugate, determinant, inverse, transpose
    MATCH(float3  , mul(float3x2(), float2()) );
    MATCH(float4  , mul(float4x3(), float3()) );
    MATCH(float2  , mul(float2x4(), float4()) );
    MATCH(float3x4, mul(float3x2(), float2x4()) );
    MATCH(float4x2, mul(float4x3(), float3x2()) );
    MATCH(float2x3, mul(float2x4(), float4x3()) );
    MATCH(float2x2, adjugate(float2x2()) );
    MATCH(float3x3, adjugate(float3x3()) );
    MATCH(float4x4, adjugate(float4x4()) );
    MATCH(float   , determinant(float2x2()) );
    MATCH(float   , determinant(float3x3()) );
    MATCH(float   , determinant(float4x4()) );
    MATCH(float2x2, inverse(float2x2()) );
    MATCH(float3x3, inverse(float3x3()) );
    MATCH(float4x4, inverse(float4x4()) );
    MATCH(float3x4, transpose(float4x3()) );
    MATCH(float4x2, transpose(float2x4()) );
    MATCH(float2x3, transpose(float3x2()) );

    // Exercise factory functions
    MATCH(float4, rotation_quat(float3(), float()) );
    MATCH(float4x4, translation_matrix(float3()) );
    MATCH(float4x4, rotation_matrix(float4()) );
    MATCH(float4x4, pose_matrix(float4(), float3()) );
    MATCH(float4x4, linalg::frustum_matrix(float(), float(), float(), float(), float(), float()) );
    MATCH(float4x4, linalg::perspective_matrix(float(), float(), float(), float()) );

    // Problematic cases, which break if the code is expressed too generically
    std::vector<int3> tris_a, tris_b;
    tris_a = std::move(tris_b);
}