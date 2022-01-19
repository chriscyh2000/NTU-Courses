#ifndef __YP_CLIQUE_GLPK_HEADER
#define __YP_CLIQUE_GLPK_HEADER
#include<glpk.h>
#include<vector>
#include<utility>
// https://www.gnu.org/software/glpk/
// http://most.ccib.rutgers.edu/glpk.pdf
namespace ypglpk{
  using std::vector,std::pair;
  bool output=0;
  constexpr double INF=1e300;
  // set whether to output verbose information about the solver (for debugging)
  void set_output(bool _output){
    output=_output;
  }
  // for interal usage. please do not call it unless you are really sure about what you are doing
  void set_constraints(glp_prob *P,const vector<vector<double>> &A,const vector<double> &b,const vector<double> &c){
    int n=c.size(),m=b.size();
    glp_set_obj_dir(P,GLP_MAX);
    glp_add_rows(P,m);
    glp_add_cols(P,n);
    for(int i=0;i<m;i++) glp_set_row_bnds(P,i+1,GLP_UP,0,b[i]);
    for(int i=0;i<n;i++) glp_set_col_bnds(P,i+1,GLP_FR,0,0);
    for(int i=0;i<n;i++) glp_set_obj_coef(P,i+1,c[i]);
    vector<int> ia(1),ja(1); vector<double> ar(1);
    for(int i=0;i<m;i++){
      for(int j=0;j<n;j++){
        if(A[i][j]==0) continue;
        ia.push_back(i+1); ja.push_back(j+1);
        ar.push_back(A[i][j]);
      }
    }
    glp_load_matrix(P,(int)ia.size()-1,ia.data(),ja.data(),ar.data());
  }
  // maximize cx subject to Ax<=b. return pair(cy,y) where y is the optimal x. if the constraints are infeasible or unbounded, return pair(-INF,{}).
  pair<double,vector<double>> linear_programming(const vector<vector<double>> &A,const vector<double> &b,const vector<double> &c){
    int n=c.size();
    glp_term_out(output?GLP_ON:GLP_OFF);
    glp_prob *lp=glp_create_prob();
    set_constraints(lp,A,b,c);
    glp_smcp parm;
    glp_init_smcp(&parm);
    parm.presolve = GLP_ON;
    if(glp_simplex(lp,&parm)){
      glp_delete_prob(lp);
      return {-INF,{}};
    }
    vector<double> ans(n);
    for(int i=0;i<n;i++) ans[i]=glp_get_col_prim(lp,i+1);
    double opt=glp_get_obj_val(lp);
    glp_delete_prob(lp);
    return {opt,ans};
  }
  // maximize cx subject to Ax<=b and xi is an integer if isint[i]. return pair(cy,y) where y is the optimal x. if the constraints are infeasible or unbounded, return pair(-INF,{}).
  pair<double,vector<double>> mixed_integer_linear_programming(const vector<vector<double>> &A,const vector<double> &b,const vector<double> &c,const vector<bool> &isint){
    int n=c.size();
    glp_term_out(output?GLP_ON:GLP_OFF);
    glp_prob *milp=glp_create_prob();
    set_constraints(milp,A,b,c);
    for(int i=0;i<n;i++){
      if(isint[i]) glp_set_col_kind(milp,i+1,GLP_IV);
    }
    glp_iocp parm;
    glp_init_iocp(&parm);
    parm.presolve = GLP_ON;
    if(glp_intopt(milp,&parm)){
      glp_delete_prob(milp);
      return {-INF,{}};
    }
    vector<double> ans(n);
    for(int i=0;i<n;i++) ans[i]=glp_mip_col_val(milp,i+1);
    double opt=glp_mip_obj_val(milp);
    glp_delete_prob(milp);
    return {opt,ans};
  }
}
#endif