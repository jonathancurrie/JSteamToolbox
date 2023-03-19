/* JSTEAM MATLAB MEX INTERFACE
 * Parallel Solver Header
 * J.Currie 2012-2015 */

#ifndef JSTEAMSOLVE_INC
#define JSTEAMSOLVE_INC

#include "include\JSteam.h"

#define NaN std::numeric_limits<double>::quiet_NaN()

using namespace JSteamDLL;

namespace JSteamMEX
{
    //Argument Enumeration
    enum{D1,D2,C1,CD,CD2};
    
    //Structure Prototypes
    typedef struct {
        char *name;
        double (JSteam::*fcnD1)(double);
        double (JSteam::*fcnD2)(double,double);
        double (JSteam::*fcnC1)(JSteam::Component);
        double (JSteam::*fcnCD)(JSteam::Component,double);
        double (JSteam::*fcnCD2)(JSteam::Component,double,double);
    } JSFcn;
    typedef struct {
        char *name;
        int n_in;
        int n_out;
        int code;
        char *sig;
    } JSUFcn;
    typedef struct {
        char *name;
        double (Mixture::*fcnM1)();
        double (Mixture::*fcnMD1)(double);
        double (Mixture::*fcnMD2)(double,double);
    } JSMFcn;

    //Parallel Solve Thermo Functions + Unit Op Interface
    class JSteamSolve
    {
    private:
        //Utility Functions
        int matchFcn(char *name, JSFcn *fcns, int &mode);
        int matchFcn(char *name, JSMFcn *fcns, int &mode);
        int matchFcn(char *name, JSUFcn*& match);
        //Parallelized Solving Routines
        void parallelSolve(JSteam *JStm, double (JSteam::*fcn)(double),double *in1, double *sol, size_t m1, size_t n1);
        void parallelSolve(JSteam *JStm, double (JSteam::*fcn)(double,double),double *in1, double *in2, double *sol, size_t m1, size_t n1);
        void parallelSolve(JSteam *JStm, double (JSteam::*fcn)(JSteam::Component,double),JSteam::Component c, double *in1, double *sol, size_t m1, size_t n1);
        void parallelSolve(JSteam *JStm, double (JSteam::*fcn)(JSteam::Component,double,double),JSteam::Component c, double *in1, double *in2, double *sol, size_t m1, size_t n1);
        void parallelSolve(double (Mixture::*fcn)(double),Mixture &JSMix, double *in1, double *sol, size_t m1, size_t n1);
        void parallelSolve(double (Mixture::*fcn)(double,double),Mixture &JSMix, double *in1, double *in2, double *sol, size_t m1, size_t n1);
        //Unit Operation Solving
        void unitOpSolve(JSteam *JStm, int model, const mxArray *prhs[], mxArray *plhs[]);
        
    public:
        void Info(JSteam *JStm, char *name, const mxArray *mix, mxArray *plhs[]);
        void Solve(JSteam *JStm, char *name, char *component, double *in1, double *in2, double *sol, size_t m1, size_t n1);   
        void SolveMix(JSteam *JStm, char *name, const mxArray *mix, double *in1, double *in2, double *sol, size_t m1, size_t n1);   
        void SolveUO(JSteam *JStm, const mxArray *prhs[], int nrhs, mxArray *plhs[]); 
        
        void Print();
        void PrintUO();
    };
}

#endif