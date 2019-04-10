#include "matrix.h"

matrix
matrix::transpose () const{

  matrix res(this->get_cols(),this->get_rows());
  for (unsigned int r=0;r<this->get_cols();r++)
    for (unsigned int c=0;c<this->get_rows();c++){
      res(r,c)=(*this)(c,r);
    }

  return res;
}
#ifdef USE_TRANSPOSED
matrix
operator* (const matrix& A, const matrix& B){

  if (A.get_cols()!=B.get_rows())
    std::cerr<<"dimension non compatible"<<std::endl;

  matrix C(A.get_rows(),B.get_cols());

  matrix A_t=A.transpose();

  for (unsigned int i=0;i<C.get_rows();i++)
    for (unsigned int j=0;j<C.get_cols();j++)
      for (unsigned int k=0;k<A.get_cols();k++)
        C(i,j)+=A_t(k,i)*B(k,j);

  return C;


}
#else
#ifdef COL_MAJOR //why isn't this version better than the one with no option?
matrix
operator* (const matrix& A, const matrix& B){

    if (A.get_cols()!=B.get_rows())
      std::cerr<<"dimension non compatible"<<std::endl;

    matrix C(A.get_rows(),B.get_cols());

    for (unsigned int j=0;j<C.get_cols();j++)
      for (unsigned int i=0;i<C.get_rows();i++)
        for (unsigned int k=0;k<A.get_cols();k++)
          C(i,j)+=A(i,k)*B(k,j);

    return C;

}
#else
matrix
operator* (const matrix& A, const matrix& B){

    if (A.get_cols()!=B.get_rows())
      std::cerr<<"dimension non compatible"<<std::endl;

    matrix C(A.get_rows(),B.get_cols());

    for (unsigned int i=0;i<C.get_rows();i++)
      for (unsigned int j=0;j<C.get_cols();j++)
        for (unsigned int k=0;k<A.get_cols();k++)
          C(i,j)+=A(i,k)*B(k,j);

    return C;

}
#endif
#endif



void
matrix::solve (matrix &rhs){



}
