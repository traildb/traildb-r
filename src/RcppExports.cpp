// This file was generated by Rcpp::compileAttributes
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// read_trails_meta
List read_trails_meta(List tdbSpec, bool verbose);
RcppExport SEXP RTraildb_read_trails_meta(SEXP tdbSpecSEXP, SEXP verboseSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< List >::type tdbSpec(tdbSpecSEXP);
    Rcpp::traits::input_parameter< bool >::type verbose(verboseSEXP);
    __result = Rcpp::wrap(read_trails_meta(tdbSpec, verbose));
    return __result;
END_RCPP
}
// read_trails
RObject read_trails(List tdbSpec, uint32_t first_trail, uint32_t last_trail, bool sample, double fraction, uint32_t start_timestamp, uint32_t stop_timestamp, std::string filter, bool verbose);
RcppExport SEXP RTraildb_read_trails(SEXP tdbSpecSEXP, SEXP first_trailSEXP, SEXP last_trailSEXP, SEXP sampleSEXP, SEXP fractionSEXP, SEXP start_timestampSEXP, SEXP stop_timestampSEXP, SEXP filterSEXP, SEXP verboseSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< List >::type tdbSpec(tdbSpecSEXP);
    Rcpp::traits::input_parameter< uint32_t >::type first_trail(first_trailSEXP);
    Rcpp::traits::input_parameter< uint32_t >::type last_trail(last_trailSEXP);
    Rcpp::traits::input_parameter< bool >::type sample(sampleSEXP);
    Rcpp::traits::input_parameter< double >::type fraction(fractionSEXP);
    Rcpp::traits::input_parameter< uint32_t >::type start_timestamp(start_timestampSEXP);
    Rcpp::traits::input_parameter< uint32_t >::type stop_timestamp(stop_timestampSEXP);
    Rcpp::traits::input_parameter< std::string >::type filter(filterSEXP);
    Rcpp::traits::input_parameter< bool >::type verbose(verboseSEXP);
    __result = Rcpp::wrap(read_trails(tdbSpec, first_trail, last_trail, sample, fraction, start_timestamp, stop_timestamp, filter, verbose));
    return __result;
END_RCPP
}