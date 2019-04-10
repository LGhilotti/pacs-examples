#ifndef HAVE_MATRIX_H
#define HAVE_MATRIX_H

#include <vector>
#include <iostream>

class
matrix
{

private :

  std::vector<double> data;
  const unsigned int rows;
  const unsigned int cols;
  std::vector<unsigned int>    p;

  inline
  unsigned int
  sub2ind (const unsigned int ir,
           const unsigned int jc) const
  { return (ir + jc * rows); }; //position in the vector data of the element (ir,jc); data stores column major.

  double &
  index (unsigned int irow, unsigned int jcol)
  { return data[sub2ind (irow, jcol)]; }; //called if you want to modify the element (irow,jcol) of the matrix.

  const double &
  const_index (unsigned int irow, unsigned int jcol) const
  { return data[sub2ind (irow, jcol)]; }; //called if you want to return the element (irow,jcol)

  bool factorized;

public :

  matrix (unsigned int size)
    : rows (size), cols (size)
  { data.resize (rows * cols, 0.0); };

  matrix (unsigned int rows_,
          unsigned int cols_)
    : rows (rows_), cols (cols_)
  { data.resize (rows * cols, 0.0); };

  matrix (matrix const &) = default;

  unsigned int
  get_rows () const { return rows; }

  unsigned int
  get_cols () const { return cols; }

  double &
  operator() (unsigned int irow, unsigned int jcol)
  { return index (irow, jcol); }; //called to modify the element (irow,jcol): it relies on the index private function.

  const double &
  operator()  (unsigned int irow, unsigned int jcol) const
  { return const_index (irow, jcol); };

  const double *
  get_data () const { return &(data[0]); };

  double *
  get_data () { return &(data[0]); };

  /// transposed matrix : B = A'
  matrix
  transpose () const;

  void
  solve (matrix &rhs);

  void
  factorize ();

};

/// matrix x matrix product : C = A * B
matrix
operator* (const matrix& A, const matrix& B);

#endif
