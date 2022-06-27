#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]

int get_mode(NumericVector y){
  return y[1];
}

// [[Rcpp::export]]
NumericVector predict_cpp(DataFrame x, DataFrame is_na, IntegerVector roots, IntegerVector yes, IntegerVector no,
                          IntegerVector missing, LogicalVector is_leaf, IntegerVector feature, NumericVector split,
                          IntegerVector decision_type, NumericVector value) {
  NumericVector prediction(x.ncol());
  for (int i = 0; i < x.ncol(); ++i) {
    NumericVector observation = x[i];
    LogicalVector observation_is_na = is_na[i];
    int node_counter = 1;
    NumericVector tree_predictions(roots.size());
    for (int node: roots) {
      while (!is_leaf[node]) {
        if (observation_is_na[feature[node]]) {
          node = missing[node];
        } else if (((decision_type[node] == 1) && (observation[feature[node]] <= split[node]))
                     || ((decision_type[node] == 2) && (observation[feature[node]] < split[node]))) {
          node = yes[node];
        } else {
          node = no[node];
        }
      }
      tree_predictions[node_counter] = value[node];
      node_counter += 1;
      //Rcout << "pred:value" << prediction[i] << " : " << value[node] << std::endl;
      //prediction[i] = ((prediction[i]*node_counter - 1) + value[node])/node_counter;
    }
    prediction[i] = get_mode(tree_predictions);

  }

  return prediction;
}

