#include <iostream>
#include <iomanip>
#include "DoubleT.h"
#include "Metaprog.h"

using namespace FPDouble;
using namespace Metaprog;
using namespace std;

template <class T,unsigned int uiN> void vTestMetaProg(void);
int main(int /*argc*/, char **/*argv[]*/)
{
    vTestMetaProg< float,15>();
    vTestMetaProg< double,50>();
    vTestMetaProg<long double,50>();
    return 0;
}
template <class T,unsigned int uiN> void vTestMetaProg(void)
{
    std::cout.precision(16);
    T tmp;
    tmp = 2.0;
    std::cout<< "2^3" << setw(15) 		<<"= " << Pow <3,T>(tmp) 						<< std::endl;
    std::cout<< "5! " << setw(15) 		<<"= " << Fact<5,T>()  							<< std::endl;
    std::cout<< "5! (Odd) " << setw(9)  <<"= " << FactOdd<5,T>() 						<< std::endl;
    std::cout<< "5! (Even)" << setw(9)  <<"= " << FactEven<5,T>()  						<< std::endl;
    std::cout<< "Pi " << setw(15) 		<<"= " << Pi<T,uiN>()  							<< std::endl;
    std::cout<< "Pi " << setw(15) 		<<"= " << M_PI 								 	<< std::endl;
    std::cout<< "Exp(1.0) " << setw(9) 	<<"= " << Exp<T,uiN>(1.0) 						<< std::endl;
    std::cout<< "exp(1.0) " << setw(9) 	<<"= " << M_E 									<< std::endl;
    std::cout<< "Sin(1.0) " << setw(9) 	<<"= " << Sin<T,uiN>(1.0) 						<< std::endl;
    std::cout<< "sin(1.0) " << setw(9) 	<<"= " << sin(1.0)  		 	 				<< std::endl;
    std::cout<< "Cos(1.0) " << setw(9) 	<<"= " << Cos<T,uiN>(1.0) 						<< std::endl;
    std::cout<< "cos(1.0) " << setw(9) 	<<"= " << cos(1.0)  		 	 				<< std::endl;
    std::cout<< "Asin(0.5)" << setw(9) 	<<"= " << Asin<T,uiN>(0.5)						<< std::endl;
    std::cout<< "asin(0.5)" << setw(9) 	<<"= " << asin(0.5)  		 					<< std::endl;
    std::cout<< "Acos(0.5)" << setw(9) 	<<"= " << Acos<T,uiN>(0.5)  	 				<< std::endl;
    std::cout<< "acos(0.5)" << setw(9) 	<<"= " << acos(0.5)  		 					<< std::endl;
    std::cout<< "Tan(0.5) " << setw(9) 	<<"= " << Tan<T,uiN>(0.5)  		 				<< std::endl;
    std::cout<< "tan(0.5) " << setw(9) 	<<"= " << tan(0.5)  		 	 				<< std::endl;
    std::cout<< "Atan(0.5)" << setw(9) 	<<"= " << Atan<T,uiN>(0.5)  	 				<< std::endl;
    std::cout<< "atan(0.5)" << setw(9)  <<"= " << atan(0.5)  		 					<< std::endl;
    std::cout<< "Sinh(1.0)" << setw(9) 	<<"= " << Sinh<T,uiN>(1.0)  	 				<< std::endl;
    std::cout<< "sinh(1.0)" << setw(9)  <<"= " << sinh(1.0)  		 					<< std::endl;
    std::cout<< "Cosh(1.0)" << setw(9) 	<<"= " << Cosh<T,uiN>(1.0)  	 				<< std::endl;
    std::cout<< "cosh(1.0)" << setw(9)  <<"= " << cosh(1.0)  		 					<< std::endl;
    std::cout<< "Tanh(1.0)" << setw(9) 	<<"= " << Tanh<T,uiN>(1.0)  	 				<< std::endl;
    std::cout<< "tanh(1.0)" << setw(9)  <<"= " << tanh(1.0)								<< std::endl;
    std::cout<< "Atanh(0.5)" << setw(8) <<"= " << Atanh<T,uiN>(0.5)	    				<< std::endl;
    std::cout<< "atanh(0.5)" << setw(8) <<"= " << atanh(0.5)  							<< std::endl;
    std::cout<< "Atan2(0.5)" << setw(8) <<"= " << Atan2<T,uiN>( 0.000182100427550697,-0.000273078158580363)	    				<< std::endl;
    std::cout<< "atan2(0.5)" << setw(8) <<"= " << atan2( 0.000182100427550697,-0.000273078158580363)  							<< std::endl;


}


