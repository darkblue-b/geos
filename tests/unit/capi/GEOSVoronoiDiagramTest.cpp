// 
// Test Suite for C-API GEOSVoronoiDiagram

#include <tut.hpp>
// geos
#include <geos_c.h>
// std
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <memory>

namespace tut
{
    //
    // Test Group
    //

    // Common data used in test cases.
    struct test_capigeosvoronoidiagram_data
    {
        GEOSGeometry* geom1_;
        GEOSGeometry* geom2_;
        GEOSWKTWriter* w_;

        static void notice(const char *fmt, ...)
        {
            std::fprintf( stdout, "NOTICE: ");

            va_list ap;
            va_start(ap, fmt);
            std::vfprintf(stdout, fmt, ap);
            va_end(ap);
        
            std::fprintf(stdout, "\n");
        }

        test_capigeosvoronoidiagram_data()
            : geom1_(0), geom2_(0)
        {
            initGEOS(notice, notice);
            w_ = GEOSWKTWriter_create();
            GEOSWKTWriter_setTrim(w_, 1);
        }

        void ensure_equals_wkt(GEOSGeometry* g, const char* exp)
        {
          GEOSNormalize(g);
          char* wkt_c = GEOSWKTWriter_write(w_, g);
          std::string out(wkt_c);
          free(wkt_c);

	  GEOSGeometry* exp_g = GEOSGeomFromWKT(exp);
	  GEOSNormalize(exp_g);
	  char* wkt_c2 = GEOSWKTWriter_write(w_, exp_g); 
	  std::string out_exp(wkt_c2);
	  free(wkt_c2);

//	  cout << "OUTPUT STRING::" << out << endl << endl;
//	  cout << "Expected STRING::" << out_exp << endl << endl;
          ensure_equals(out, out_exp);
	  GEOSGeom_destroy(exp_g);
        }

        ~test_capigeosvoronoidiagram_data()
        {
            GEOSGeom_destroy(geom1_);
            GEOSGeom_destroy(geom2_);
            GEOSWKTWriter_destroy(w_);
            geom1_ = 0;
            geom2_ = 0;
            finishGEOS();
        }

    };

    typedef test_group<test_capigeosvoronoidiagram_data> group;
    typedef group::object object;

    group test_capigeosvoronoidiagram_group("capi::GEOSVoronoiDiagram");

    //
    // Test Cases
    //

    // Single point
    template<>
    template<>
    void object::test<1>()
    {
	    geom1_ = GEOSGeomFromWKT("POINT(10 20)");

	    geom2_ = GEOSVoronoiDiagram(geom1_,0, 0);
	    ensure_equals ( GEOSisEmpty(geom2_), 1 );
	    ensure_equals ( GEOSGeomTypeId(geom2_), GEOS_GEOMETRYCOLLECTION );

	    GEOSGeom_destroy(geom2_);
	    geom2_ = GEOSVoronoiDiagram(geom1_, 0, 1); 
	    char* wkt_c = GEOSWKTWriter_write(w_, geom2_);
	    std::string out(wkt_c);
	    free(wkt_c);
	    ensure_equals(out,"MULTILINESTRING EMPTY");
    }

    //More points:
    template<>
    template<>
    void object::test<2>()
    {
	    geom1_ = GEOSGeomFromWKT("MULTIPOINT ((280 300), (420 330), (380 230), (320 160))");

	    geom2_ = GEOSVoronoiDiagram(geom1_,0,0);
	    ensure_equals_wkt(geom2_ ,"GEOMETRYCOLLECTION (POLYGON ((110 175.71428571428572, 110 500, 310.35714285714283 500, 353.515625 298.59375, 306.875 231.96428571428572, 110 175.71428571428572)), POLYGON ((590 204, 590 -10, 589.1666666666666 -10, 306.875 231.96428571428572, 353.515625 298.59375, 590 204)), POLYGON ((110 -10, 110 175.71428571428572, 306.875 231.96428571428572, 589.1666666666666 -10, 110 -10)), POLYGON ((310.35714285714283 500, 590 500, 590 204, 353.515625 298.59375, 310.35714285714283 500)))" );

	    GEOSGeom_destroy(geom2_);
	    geom2_ = GEOSVoronoiDiagram(geom1_, 0, 1); 
	    char* wkt_c = GEOSWKTWriter_write(w_, geom2_);
	    std::string out(wkt_c);
	    free(wkt_c);
	    ensure_equals(out, "MULTILINESTRING ((310.3571428571428 500, 353.515625 298.59375), (353.515625 298.59375, 306.875 231.9642857142857), (306.875 231.9642857142857, 110 175.7142857142857), (589.1666666666666 -10, 306.875 231.9642857142857), (353.515625 298.59375, 590 204))");
    }
    //Larger number of points:
    template<>
    template<>
    void object::test<3>()
    {
	    geom1_ = GEOSGeomFromWKT("MULTIPOINT ((170 270), (270 270), (230 310), (180 330), (250 340), (315 318), (330 260), (240 170), (220 220), (270 220))");

	    geom2_ = GEOSVoronoiDiagram(geom1_,0,0);
	    ensure_equals_wkt(geom2_,"GEOMETRYCOLLECTION (POLYGON ((0 329.1666666666667, 0 510, 190 510, 213.94736842105263 342.36842105263156, 195.625 296.5625, 0 329.1666666666667)), POLYGON ((0 76.50000000000001, 0 329.1666666666667, 195.625 296.5625, 216 266, 88.33333333333333 138.33333333333334, 0 76.50000000000001)), POLYGON ((216 266, 195.625 296.5625, 213.94736842105263 342.36842105263156, 267 307, 225 265, 216 266)), POLYGON ((245 245, 225 265, 267 307, 275.9160583941606 309.54744525547443, 303.1666666666667 284, 296.6666666666667 245, 245 245)), POLYGON ((225 265, 245 245, 245 201, 88.33333333333333 138.33333333333334, 216 266, 225 265)), POLYGON ((0 0, 0 76.50000000000001, 88.33333333333333 138.33333333333334, 245 201, 380 120, 500 0, 0 0)), POLYGON ((190 510, 343.76153846153846 510, 275.9160583941606 309.54744525547443, 267 307, 213.94736842105263 342.36842105263156, 190 510)), POLYGON ((245 201, 245 245, 296.6666666666667 245, 380 120, 245 201)), POLYGON ((343.76153846153846 510, 500 510, 500 334.9051724137931, 303.1666666666667 284, 275.9160583941606 309.54744525547443, 343.76153846153846 510)), POLYGON ((500 334.9051724137931, 500 0, 380 120, 296.6666666666667 245, 303.1666666666667 284, 500 334.9051724137931)))");

	    GEOSGeom_destroy(geom2_);
	    geom2_ = GEOSVoronoiDiagram(geom1_, 0, 1);
	    char* wkt_c = GEOSWKTWriter_write(w_, geom2_); 
	    std::string out(wkt_c);
	    free(wkt_c);
	    ensure_equals(out, 
"MULTILINESTRING ((190 510, 213.9473684210526 342.3684210526316), (213.9473684210526 342.3684210526316, 195.625 296.5625), (195.625 296.5625, 0 329.1666666666667), (195.625 296.5625, 216 266), (216 266, 88.33333333333333 138.3333333333333), (88.33333333333333 138.3333333333333, 0 76.50000000000001), (213.9473684210526 342.3684210526316, 267 307), (267 307, 225 265), (225 265, 216 266), (245 245, 225 265), (267 307, 275.9160583941606 309.5474452554744), (275.9160583941606 309.5474452554744, 303.1666666666667 284), (303.1666666666667 284, 296.6666666666667 245), (296.6666666666667 245, 245 245), (245 245, 245 201), (245 201, 88.33333333333333 138.3333333333333), (245 201, 380 120), (380 120, 500 0), (343.7615384615385 510, 275.9160583941606 309.5474452554744), (296.6666666666667 245, 380 120), (500 334.9051724137931, 303.1666666666667 284))"
      );
    }
    //Test with non-zero Tolerance value
    template<>
    template<>
    void object::test<4>()
    {
	    geom1_ = GEOSGeomFromWKT("MULTIPOINT ((150 210), (210 270), (150 220), (220 210), (215 269))");

	    geom2_ = GEOSVoronoiDiagram(geom1_,10,0);
	    ensure_equals_wkt(geom2_,"GEOMETRYCOLLECTION (POLYGON ((290 252.5, 290 140, 185 140, 185 215, 187.9268292682927 235.4878048780488, 290 252.5)), POLYGON ((80 215, 80 340, 100.83333333333336 340, 187.9268292682927 235.4878048780488, 185 215, 80 215)), POLYGON ((80 140, 80 215, 185 215, 185 140, 80 140)), POLYGON ((100.83333333333336 340, 290 340, 290 252.5, 187.9268292682927 235.4878048780488, 100.83333333333336 340)))");

	    GEOSGeom_destroy(geom2_);
	    geom2_ = GEOSVoronoiDiagram(geom1_, 10, 1);
	    char* wkt_c = GEOSWKTWriter_write(w_, geom2_); 
	    std::string out(wkt_c);
	    free(wkt_c);
	    ensure_equals(out,
"MULTILINESTRING ((185 215, 187.9268292682927 235.4878048780488), (187.9268292682927 235.4878048780488, 290 252.5), (185 140, 185 215), (185 215, 80 215), (100.8333333333334 340, 187.9268292682927 235.4878048780488))"
      );
    }
    template<>
    template<>
    void object::test<5>()
    {
	    geom1_ = GEOSGeomFromWKT("MULTIPOINT ((40 420), (50 420), (210 290), (300 360), (350 150), (170 70), (134 135) ,(305 359), (351 145), (175 71))");

	    geom2_ = GEOSVoronoiDiagram(geom1_,10,0);
	    ensure_equals_wkt(geom2_, "GEOMETRYCOLLECTION (POLYGON ((-310 146.559649122807, -310 770, 45 770, 45 263.64736842105265, -310 146.559649122807)), POLYGON ((-310 -153.37692307692305, -310 146.559649122807, 45 263.64736842105265, 59.16911764705881 267.8235294117647, 239.43506493506493 179.43506493506493, 241.34156378600824 151.98148148148147, -310 -153.37692307692305)), POLYGON ((45 770, 266.20000000000005 770, 181.94323144104806 418.9301310043668, 59.16911764705881 267.8235294117647, 45 263.64736842105265, 45 770)), POLYGON ((59.16911764705881 267.8235294117647, 181.94323144104806 418.9301310043668, 311.875 251.875, 239.43506493506493 179.43506493506493, 59.16911764705881 267.8235294117647)), POLYGON ((-310 -280, -310 -153.37692307692305, 241.34156378600824 151.98148148148147, 433.3333333333333 -280, -310 -280)), POLYGON ((266.20000000000005 770, 701 770, 701 344.5238095238096, 311.875 251.875, 181.94323144104806 418.9301310043668, 266.20000000000005 770)), POLYGON ((701 344.5238095238096, 701 -280, 433.3333333333333 -280, 241.34156378600824 151.98148148148147, 239.43506493506493 179.43506493506493, 311.875 251.875, 701 344.5238095238096)))");

	    GEOSGeom_destroy(geom2_);
	    geom2_ = GEOSVoronoiDiagram(geom1_, 10, 1);
	    char* wkt_c = GEOSWKTWriter_write(w_, geom2_);
	    std::string out(wkt_c);
	    free(wkt_c);
	    ensure_equals(out,
"MULTILINESTRING ((45 770, 45 263.6473684210526), (45 263.6473684210526, -310 146.559649122807), (45 263.6473684210526, 59.16911764705881 267.8235294117647), (59.16911764705881 267.8235294117647, 239.4350649350649 179.4350649350649), (239.4350649350649 179.4350649350649, 241.3415637860082 151.9814814814815), (241.3415637860082 151.9814814814815, -310 -153.376923076923), (266.2 770, 181.9432314410481 418.9301310043668), (181.9432314410481 418.9301310043668, 59.16911764705881 267.8235294117647), (181.9432314410481 418.9301310043668, 311.875 251.875), (311.875 251.875, 239.4350649350649 179.4350649350649), (241.3415637860082 151.9814814814815, 433.3333333333333 -280), (701 344.5238095238096, 311.875 251.875))"
	    );
    }
    template<>
    template<>
    void object::test<6>()
    {
	    geom1_ = GEOSGeomFromWKT("MULTIPOINT ((123 245), (165 313), (240 310), (260 260), (180 210), (240 210))");
	    geom2_ = GEOSVoronoiDiagram(geom1_,0,1);

	    char* wkt_c = GEOSWKTWriter_write(w_, geom2_);
	    std::string out(wkt_c);
	    free(wkt_c);
	    ensure_equals(out,
"MULTILINESTRING ((-14 376.5882352941176, 172.3651328095773 261.4803591470258), (172.3651328095773 261.4803591470258, 56.63157894736844 73), (172.3651328095773 261.4803591470258, 200.6640625 265.6015625), (200.6640625 265.6015625, 201 265.4), (201 265.4, 210 251), (210 251, 210 73), (208.04 450, 200.6640625 265.6015625), (397 343.8, 201 265.4), (210 251, 397 176.2))"
	    );
    }
} // namespace tut

