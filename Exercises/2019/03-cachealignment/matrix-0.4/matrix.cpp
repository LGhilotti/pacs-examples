#include "matrix.h"
#include <cassert>
#include <algorithm>
#include <ctime>

//in order to use this function, you have to:
// in the linking phase indicate -L$mkOpenblasLib -lopenblas
//this is only precompiled executables, does not have headers (nothing to #include)

#if defined (USE_DGEMM)
#define dgemm dgemm_ // ?? where do you use this definition? what for?
#define DGEMM dgemm_

extern "C"
{
  void
  dgemm (const char *TRANSA, const char *TRANSB, const int *M,
         const int *N, const int *K, const double *ALPHA,
         const double *A, const int *LDA, const double *B,
         const int *LDB, const double *BETA, double *C,
         const int *LDC);
}
#endif

matrix
matrix::transpose () const
{
  matrix retval (get_cols (), get_rows ());
  unsigned int i, j;
  for (j = 0; j < retval.get_cols (); ++j)
    for (i = 0; i < retval.get_rows (); ++i)
      retval (i, j) = const_index (j, i);
  return (retval);
}

#if defined (MAKE_TMP_TRANSP)
matrix
operator* (const matrix& A, const matrix& B)
{
  unsigned int i, j, k;
  assert (A.get_cols () == B.get_rows ());
  matrix retval (A.get_rows (), B.get_cols ());
  matrix tmp = A.transpose ();
  for (i = 0; i < retval.get_rows (); ++i)
    for (j = 0; j < retval.get_cols (); ++j)
      for (k = 0; k < A.get_cols (); ++k)
        retval(i,j) += tmp(k,i) * B(k,j);
  return (retval);
}
#elif defined (USE_EIGEN_MAP_PROD)
#include <Eigen/Dense>
matrix
operator* (const matrix& A, const matrix& B)
{
  assert (A.get_cols () == B.get_rows ());

  Eigen::Map<const Eigen::MatrixXd>
    eigen_A (A.get_data (), A.get_rows (), A.get_cols ());

  Eigen::Map<const Eigen::MatrixXd>
    eigen_B (B.get_data (), B.get_rows (), B.get_cols ());

  matrix retval (A.get_rows (), B.get_cols ());

  Eigen::Map<Eigen::MatrixXd>
    eigen_retval (retval.get_data (), A.get_rows (), B.get_cols ());

  eigen_retval = eigen_A * eigen_B;

  return (retval);
}
#elif defined (USE_DGEMM)
matrix
operator* (const matrix& A, const matrix& B)
{

  int M = A.get_rows ();
  int N = B.get_cols ();
  int K = A.get_cols ();
  assert (K == B.get_rows ());

  char ntr = 'n';
  double one = 1.0;
  double zero = 0.0;

  matrix retval (M, N);

  dgemm (&ntr, &ntr, &M, &N, &K,
         &one, A.get_data (), &M,
         B.get_data (), &K, &zero,
         retval.get_data (), &M);

  return (retval);
}
#else
matrix
operator* (const matrix& A, const matrix& B)
{
  unsigned int i, j, k;
  assert (A.get_cols () == B.get_rows ());
  matrix retval (A.get_rows (), B.get_cols ());
  for (i = 0; i < retval.get_rows (); ++i)
    for (j = 0; j < retval.get_cols (); ++j)
      for (k = 0; k < A.get_cols (); ++k)
        retval(i,j) += A(i,k) * B(k,j);
  return (retval);
}
#endif

void
matrix::solve (matrix &rhs)
{
  unsigned int ii, jj, kk;
  double f;

  // Factorize
  if (! factorized)
    { factorize (); std::cout << "factorize !" << std::endl; }

  double *b = rhs.get_data ();
  // Do Forward Substitution
  std::cout << "fwdsub !" << std::endl;
  for (ii = 0; ii < get_rows (); ++ii)
    {
      f = b[p[ii]];
      for (kk = 0; kk < ii; ++kk)
        f -= index (p[ii], kk) * b[p[kk]];
      b[p[ii]] = f;
    }

  // Do Backward Substitution
  std::cout << "bwdsub !" << std::endl;
  for (jj = 1; jj <= get_rows (); ++jj)
    {
      ii = get_rows () - jj;
      f = b[p[ii]];
      for (kk = ii+1; kk < get_cols (); ++kk)
          f -= index (p[ii], kk) * b[p[kk]];
      b[p[ii]] = f / index (p[ii], ii);
    }
};

void
matrix::factorize ()
{
  p.resize (rows, 0);
  for (int ii = 0; ii < rows; ++ii)
    p[ii] = ii;

  int m = this->get_rows ();
  int n = this->get_cols ();
  int ii, jj, kk;
  double pivot = 0.,
    maxpivot = 0.;
  int imaxpivot = 0;

  assert (m == n);
  for (ii = 0; ii < m-1; ++ii) //except the last column because there is nothing under diagonal element
    {
      maxpivot = index (p[ii], ii);
      imaxpivot = ii;
      for (kk = ii+1; kk < m; ++kk)
        if (index (p[kk], ii) > maxpivot)
          {
            maxpivot = index (p[kk], ii);
            imaxpivot = kk;
          }

      if (imaxpivot != ii)
        std::swap (p[ii], p[imaxpivot]);

      pivot = index (p[ii], ii); //index returns a reference?
      for (jj = ii+1; jj < m; ++jj)
        {
          index (p[jj],ii) = index (p[jj],ii) / pivot;

          for (kk = ii+1; kk < n; ++kk)
            index (p[jj],kk) +=
              -index (p[ii],kk) * index (p[jj],ii);

        }
    }
  factorized = true;
};
