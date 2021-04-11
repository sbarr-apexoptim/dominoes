#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <OsiSolverInterface.hpp>
#include <OsiCbcSolverInterface.hpp>
#include <OsiClpSolverInterface.hpp>
#include "CoinPackedMatrix.hpp"
#include "CoinPackedVector.hpp"

using namespace std;

int main(void) {
  //OsiSolverInterface *si = new OsiClpSolverInterface();
  OsiSolverInterface *si = new OsiCbcSolverInterface();

  double large_val = 1e8;

  int n_sets = 1;
  //int n_sets = 48;
  int n_types = 100; // 55 Dominoes in flipped and unflipped orientations
  int **phi;
  phi = new int*[n_types]; // Number of dots on each side of this type of domino
  int cnt = 0;
  for(int i=0;i<n_types;i++) {
    phi[i] = new int[2];
  }

  for(int i=0;i<10;i++) {
    phi[i][0] = i;
    phi[i][1] = i;
  }
  cnt = 10;
  for(int s=0;s<2;s++) {
    for(int i=0;i<10;i++) {
      for(int j=0;j<10;j++) {
        if(j > i) {
          if(s == 0) {
            phi[cnt][0] = i;
            phi[cnt][1] = j;
          } else {
            phi[cnt][0] = j;
            phi[cnt][1] = i;
          }
          cnt++;
        }
      }
    }
  }

  ifstream in("mary.dat");
  string line;
  stringstream ss;

  //int imax = 60;
  //int jmax = 88;
  int imax = 10;
  int jmax = 11;
  int **brightness;
  brightness = new int*[imax];
  for(int i=0;i<imax;i++) {
    brightness[i] = new int[jmax];
    getline(in, line);
    ss.clear();
    ss.str(std::string());
    ss << line;
    for(int j=0;j<jmax;j++) {
      ss >> brightness[i][j];
    }
  }

//  int n_cols = imax * jmax + // Count of tiles occupying cell i,j
  int n_cols = (imax-1) * jmax * n_types + // Whether tile k is on vertical edge i,j
               imax * (jmax-1) * n_types + // Whether tile k is on horizontal edge i,j
               imax * jmax + // Number of dots on cell i,j
               imax * jmax + // Difference between number of dots and brightness in cell
               imax * jmax + // t1 i,j
               imax * jmax;  // t2 i,j

//  int n_rows = imax * jmax + // Number of tiles on cell i,j equal to 1
  int n_rows = imax * jmax + // Number of tiles on cell i,j equal to sum of tiles on adjacent edges
               imax * jmax + // Number of dots on cell i,j equal to sum of tile dots on adjacent edges
               imax * jmax + // Difference between number of dots and brightness in cell
               10 + // These are the tiles which are symmetric, so the flipped version is the same as the unflipped.
               45 + // Optimizer can decide to place a flipped or unflipped tile on an edge, but they are really the same tile, so the sum of those tiles should equal the number of sets of dominoes
               imax * jmax; //  Absolute values

  double *objective    = new double[n_cols];//the objective coefficients
  double *col_lb       = new double[n_cols];//the column lower bounds
  double *col_ub       = new double[n_cols];//the column upper bounds

  cnt = 0;
/*
  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax;j++) {
      col_lb[cnt] = 0.0;
      col_ub[cnt] = 9.0;
      objective[cnt] = 0.0;
      cnt++;
    }
  }
*/


  for (int i=0;i<imax-1;i++) {
    for (int j=0;j<jmax;j++) {
      for(int k=0;k<n_types;k++){
        col_lb[cnt] = 0.0;
        col_ub[cnt] = 1.0;
        objective[cnt] = 0.0;
        cnt++;
      }
    }
  }

  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax-1;j++) {
      for(int k=0;k<n_types;k++){
        col_lb[cnt] = 0.0;
        col_ub[cnt] = 1.0;
        objective[cnt] = 0.0;
        cnt++;
      }
    }
  }

  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax;j++) {
      col_lb[cnt] = 0.0;
      col_ub[cnt] = 9.0;
      objective[cnt] = 0.0;
      cnt++;
    }
  }

  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax;j++) {
      col_lb[cnt] = -9.0;
      col_ub[cnt] = 9.0;
      objective[cnt] = 0.0;
      cnt++;
    }
  }

  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax;j++) {
      col_lb[cnt] = 0.0;
      col_ub[cnt] = 9.0;
      objective[cnt] = 1.0;
      cnt++;
    }
  }

  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax;j++) {
      col_lb[cnt] = 0.0;
      col_ub[cnt] = 9.0;
      objective[cnt] = 1.0;
      cnt++;
    }
  }

  double *row_lb = new double[n_rows]; //the row lower bounds
  double *row_ub = new double[n_rows]; //the row upper bounds

  cnt = 0;
//  for (int i=0;i<imax;i++) {
//    for (int j=0;j<jmax;j++) {
//      row_lb[cnt] = 1.0;
//      row_ub[cnt] = 1.0;
//      cnt++;
//    }
//  }

  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax;j++) {
      row_lb[cnt] = 1.0;
      row_ub[cnt] = 1.0;
      cnt++;
    }
  }
  int non_zeros1 = ((2*(imax-1)*jmax*n_types) + (2*imax*(jmax-1)*n_types));
  cout << non_zeros1 << endl;
  int *ia1, *ja1;
  double *ar1;
  ia1 = new int[non_zeros1];
  ja1 = new int[non_zeros1];
  ar1 = new double[non_zeros1];

  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax;j++) {
      row_lb[cnt] = 0.0;
      row_ub[cnt] = 0.0;
      cnt++;
    }
  }
  int non_zeros2 = ((2*(imax-1)*jmax*n_types) + (2*imax*(jmax-1)*n_types));
  int *ia2, *ja2;
  double *ar2;
  ia2 = new int[non_zeros2];
  ja2 = new int[non_zeros2];
  ar2 = new double[non_zeros2];

  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax;j++) {
      row_lb[cnt] = brightness[i][j];
      row_ub[cnt] = brightness[i][j];
      cnt++;
    }
  }
  int non_zeros3 = (2*imax*jmax);
  int *ia3, *ja3;
  double *ar3;
  ia3 = new int[non_zeros3];
  ja3 = new int[non_zeros3];
  ar3 = new double[non_zeros3];

  for (int n=0;n<10;n++) {
    row_lb[cnt] = n_sets;
    row_ub[cnt] = n_sets;
    cnt++;
  }
  int non_zeros4 = 10*(((imax-1)*jmax) + (imax*(jmax-1)));
  int *ia4, *ja4;
  double *ar4;
  ia4 = new int[non_zeros4];
  ja4 = new int[non_zeros4];
  ar4 = new double[non_zeros4];

  for (int n=10;n<55;n++) {
    row_lb[cnt] = n_sets;
    row_ub[cnt] = n_sets;
    cnt++;
  }
  int non_zeros5 = 45*(2*((imax-1)*jmax) + (2*imax*(jmax-1)));
  int *ia5, *ja5;
  double *ar5;
  ia5 = new int[non_zeros5];
  ja5 = new int[non_zeros5];
  ar5 = new double[non_zeros5];

  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax;j++) {
      row_lb[cnt] = 0;
      row_ub[cnt] = 0;
      cnt++;
    }
  }
  int non_zeros6 = (3*imax*jmax);
  int *ia6, *ja6;
  double *ar6;
  ia6 = new int[non_zeros6];
  ja6 = new int[non_zeros6];
  ar6 = new double[non_zeros6];
    
  CoinPackedMatrix *matrix =  new CoinPackedMatrix(false,0,0);
  matrix->setDimensions(0, n_cols);

  cnt = 0;
  int n=0, nc=0;

/*
  cout << "first loop..." << endl;
  // Only one tile per cell
  for(int i=0;i<imax;i++) {
    for(int j=0;j<jmax;j++) {
      n = 0;
      // Cycle through cell count variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          if(i == i2 && j == j2) {
            ia[cnt] = nc;
            ja[cnt] = n;
            ar[cnt] = 1.0;
            cnt++;
          }
          n++;
        }
      }
      // Cycle through vertical edge variables
      for(int i2=0;i2<imax-1;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          for(int k2=0;k2<n_types;k2++) {
            n++;
          }
        }
      }
      // Cycle through horizontal edge variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax-1;j2++) {
          for(int k2=0;k2<n_types;k2++) {
            n++;
          }
        }
      }
      // Cycle through number of dots variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          n++;
        }
      }
      // Cycle through x variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          n++;
        }
      }
      // Cycle through t1 variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          n++;
        }
      }
      // Cycle through t2 variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          n++;
        }
      }
      nc++;
    }
  }
*/

  cout << "second loop..." << endl;
  // Cells on each tile equal to sum of those on adjacent edges
  for(int i=0;i<imax;i++) {
    for(int j=0;j<jmax;j++) {
      int cnt = 0;
      int non_zeros = 0;
      n = 0;
      //// Cycle through cell count variables
      //for(int i2=0;i2<imax;i2++) {
      //  for(int j2=0;j2<jmax;j2++) {
      //    if(i == i2 && j == j2) {
      //      ia1[cnt] = nc;
      //      ja1[cnt] = n;
      //      ar1[cnt] = -1.0;
      //      cnt++;
      //    }
      //    n++;
      //  }
      //}
      // Cycle through vertical edge variables
      for(int i2=0;i2<imax-1;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          for(int k2=0;k2<n_types;k2++) {
            if(i == i2 && j == j2) {
              ia1[cnt] = nc;
              ja1[cnt] = n;
              ar1[cnt] = 1.0;
              non_zeros++;
              cnt++;
            //} else if(i > 0 && i == i2+1 && j == j2) {
            } else if(i == i2+1 && j == j2) {
              ia1[cnt] = nc;
              ja1[cnt] = n;
              ar1[cnt] = 1.0;
              non_zeros++;
              cnt++;
            }
            n++;
          }
        }
      }
      // Cycle through horizontal edge variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax-1;j2++) {
          for(int k2=0;k2<n_types;k2++) {
            if(i == i2 && j == j2) {
              ia1[cnt] = nc;
              ja1[cnt] = n;
              ar1[cnt] = 1.0;
              non_zeros++;
              cnt++;
            //} else if(j > 0 && i == i2 && j == j2+1) {
            } else if(i == i2 && j == j2+1) {
              ia1[cnt] = nc;
              ja1[cnt] = n;
              ar1[cnt] = 1.0;
              non_zeros++;
              cnt++;
            }
            n++;
          }
        }
      }
      // Cycle through number of dots variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          n++;
        }
      }
      // Cycle through x variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          n++;
        }
      }
      // Cycle through t1 variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          n++;
        }
      }
      // Cycle through t2 variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          n++;
        }
      }
      CoinPackedVector row1(non_zeros,ja1,ar1);
      matrix->appendRow(row1);
      nc++;
    }
  }

  cout << "third loop..." << endl;
  // Number of dots constraint
  for(int i=0;i<imax;i++) {
    for(int j=0;j<jmax;j++) {
      int cnt = 0;
      int non_zeros = 0;
      n = 0;
      //// Cycle through cell count variables
      //for(int i2=0;i2<imax;i2++) {
      //  for(int j2=0;j2<jmax;j2++) {
      //    n++;
      //  }
      //}
      // Cycle through vertical edge variables
      for(int i2=0;i2<imax-1;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          for(int k2=0;k2<n_types;k2++) {
            if(i == i2 && j == j2) {
              ia2[cnt] = nc;
              ja2[cnt] = n;
              ar2[cnt] = phi[k2][0];
              non_zeros++;
              cnt++;
            //} else if(i > 0 && i == i2+1 && j == j2) {
            } else if(i == i2+1 && j == j2) {
              ia2[cnt] = nc;
              ja2[cnt] = n;
              ar2[cnt] = phi[k2][1];
              non_zeros++;
              cnt++;
            }
            n++;
          }
        }
      }
      // Cycle through horizontal edge variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax-1;j2++) {
          for(int k2=0;k2<n_types;k2++) {
            if(i == i2 && j == j2) {
              ia2[cnt] = nc;
              ja2[cnt] = n;
              ar2[cnt] = phi[k2][0];
              non_zeros++;
              cnt++;
            //} else if(j > 0 && i == i2 && j == j2+1) {
            } else if(i == i2 && j == j2+1) {
              ia2[cnt] = nc;
              ja2[cnt] = n;
              ar2[cnt] = phi[k2][1];
              non_zeros++;
              cnt++;
            }
            n++;
          }
        }
      }
      // Cycle through number of dots variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          if(i == i2 && j == j2) {
            ia2[cnt] = nc;
            ja2[cnt] = n;
            ar2[cnt] = -1.0;
            non_zeros++;
            cnt++;
          }
          n++;
        }
      }
      // Cycle through x variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          n++;
        }
      }
      // Cycle through t1 variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          n++;
        }
      }
      // Cycle through t2 variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          n++;
        }
      }
      CoinPackedVector row2(non_zeros,ja2,ar2);
      matrix->appendRow(row2);
      nc++;
    }
  }

  cout << "fourth loop..." << endl;
  // Difference in brightness contraint
  for(int i=0;i<imax;i++) {
    for(int j=0;j<jmax;j++) {
      int non_zeros = 0;
      cnt = 0;
      n = 0;
      //// Cycle through cell count variables
      //for(int i2=0;i2<imax;i2++) {
      //  for(int j2=0;j2<jmax;j2++) {
      //    n++;
      //  }
      //}
      // Cycle through vertical edge variables
      for(int i2=0;i2<imax-1;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          for(int k2=0;k2<n_types;k2++) {
            n++;
          }
        }
      }
      // Cycle through horizontal edge variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax-1;j2++) {
          for(int k2=0;k2<n_types;k2++) {
            n++;
          }
        }
      }
      // Cycle through number of dots variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          if(i == i2 && j == j2) {
            ia3[cnt] = nc;
            ja3[cnt] = n;
            ar3[cnt] = 1.0;
            non_zeros++;
            cnt++;
          }
          n++;
        }
      }
      // Cycle through x variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          if(i == i2 && j == j2) {
            ia3[cnt] = nc;
            ja3[cnt] = n;
            ar3[cnt] = -1.0;
            non_zeros++;
            cnt++;
          }
          n++;
        }
      }
      // Cycle through t1 variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          n++;
        }
      }
      // Cycle through t2 variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          n++;
        }
      }
      CoinPackedVector row3(non_zeros,ja3,ar3);
      matrix->appendRow(row3);
      nc++;
    }
  }

  cout << "fifth loop..." << endl;
  // Symmetric tiles constraint
  for(int k=0;k<10;k++) {
    int non_zeros = 0;
    cnt = 0;
    n = 0;
    //// Cycle through cell count variables
    //for(int i2=0;i2<imax;i2++) {
    //  for(int j2=0;j2<jmax;j2++) {
    //    n++;
    //  }
    //}
    // Cycle through vertical edge variables
    for(int i2=0;i2<imax-1;i2++) {
      for(int j2=0;j2<jmax;j2++) {
        for(int k2=0;k2<n_types;k2++) {
          if(k == k2) {
            ia4[cnt] = nc;
            ja4[cnt] = n;
            ar4[cnt] = 1.0;
            non_zeros++;
            cnt++;
          }
          n++;
        }
      }
    }
    // Cycle through horizontal edge variables
    for(int i2=0;i2<imax;i2++) {
      for(int j2=0;j2<jmax-1;j2++) {
        for(int k2=0;k2<n_types;k2++) {
          if(k == k2) {
            ia4[cnt] = nc;
            ja4[cnt] = n;
            ar4[cnt] = 1.0;
            non_zeros++;
            cnt++;
          }
          n++;
        }
      }
    }
    // Cycle through number of dots variables
    for(int i2=0;i2<imax;i2++) {
      for(int j2=0;j2<jmax;j2++) {
        n++;
      }
    }
    // Cycle through x variables
    for(int i2=0;i2<imax;i2++) {
      for(int j2=0;j2<jmax;j2++) {
        n++;
      }
    }
    // Cycle through t1 variables
    for(int i2=0;i2<imax;i2++) {
      for(int j2=0;j2<jmax;j2++) {
        n++;
      }
    }
    // Cycle through t2 variables
    for(int i2=0;i2<imax;i2++) {
      for(int j2=0;j2<jmax;j2++) {
        n++;
      }
    }
    CoinPackedVector row4(non_zeros,ja4,ar4);
    matrix->appendRow(row4);
    nc++;
  }

  cout << "sixth loop..." << endl;
  // Non-symmetric tiles constraint
  for(int k=10;k<55;k++) {
    int non_zeros = 0;
    cnt = 0;
    n = 0;
    //// Cycle through cell count variables
    //for(int i2=0;i2<imax;i2++) {
    //  for(int j2=0;j2<jmax;j2++) {
    //    n++;
    //  }
    //}
    // Cycle through vertical edge variables
    for(int i2=0;i2<imax-1;i2++) {
      for(int j2=0;j2<jmax;j2++) {
        for(int k2=0;k2<n_types;k2++) {
          if(k2 > 9) {
            if(k == k2 || k == k2-45) {
              ia5[cnt] = nc;
              ja5[cnt] = n;
              ar5[cnt] = 1.0;
              non_zeros++;
              cnt++;
            }
          }
          n++;
        }
      }
    }
    // Cycle through horizontal edge variables
    for(int i2=0;i2<imax;i2++) {
      for(int j2=0;j2<jmax-1;j2++) {
        for(int k2=0;k2<n_types;k2++) {
          if(k2 > 9) {
            if(k == k2 || k == k2-45) {
              ia5[cnt] = nc;
              ja5[cnt] = n;
              ar5[cnt] = 1.0;
              non_zeros++;
              cnt++;
            }
          }
          n++;
        }
      }
    }
    // Cycle through number of dots variables
    for(int i2=0;i2<imax;i2++) {
      for(int j2=0;j2<jmax;j2++) {
        n++;
      }
    }
    // Cycle through x variables
    for(int i2=0;i2<imax;i2++) {
      for(int j2=0;j2<jmax;j2++) {
        n++;
      }
    }
    // Cycle through t1 variables
    for(int i2=0;i2<imax;i2++) {
      for(int j2=0;j2<jmax;j2++) {
        n++;
      }
    }
    // Cycle through t2 variables
    for(int i2=0;i2<imax;i2++) {
      for(int j2=0;j2<jmax;j2++) {
        n++;
      }
    }
    CoinPackedVector row5(non_zeros,ja5,ar5);
    matrix->appendRow(row5);
    nc++;
  }

  cout << "seventh loop..." << endl;
  // Absolute value constraint
  for(int i=0;i<imax;i++) {
    for(int j=0;j<jmax;j++) {
      int non_zeros = 0;
      cnt = 0;
      n = 0;
      //// Cycle through cell count variables
      //for(int i2=0;i2<imax;i2++) {
      //  for(int j2=0;j2<jmax;j2++) {
      //    n++;
      //  }
      //}
      // Cycle through vertical edge variables
      for(int i2=0;i2<imax-1;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          for(int k2=0;k2<n_types;k2++) {
            n++;
          }
        }
      }
      // Cycle through horizontal edge variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax-1;j2++) {
          for(int k2=0;k2<n_types;k2++) {
            n++;
          }
        }
      }
      // Cycle through number of dots variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          n++;
        }
      }
      // Cycle through x variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          if(i == i2 && j == j2) {
            ia6[cnt] = nc;
            ja6[cnt] = n;
            ar6[cnt] = -1.0;
            non_zeros++;
            cnt++;
          }
          n++;
        }
      }
      // Cycle through t1 variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          if(i == i2 && j == j2) {
            ia6[cnt] = nc;
            ja6[cnt] = n;
            ar6[cnt] = 1.0;
            non_zeros++;
            cnt++;
          }
          n++;
        }
      }
      // Cycle through t2 variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          if(i == i2 && j == j2) {
            ia6[cnt] = nc;
            ja6[cnt] = n;
            ar6[cnt] = -1.0;
            non_zeros++;
            cnt++;
          }
          n++;
        }
      }
      CoinPackedVector row6(non_zeros,ja6,ar6);
      matrix->appendRow(row6);
      nc++;
    }
  }

  cout << "Loading problem" << endl;
  //load the problem to OSI
  si->loadProblem(*matrix, col_lb, col_ub, objective, row_lb, row_ub);
  cout << "Done." << endl;

  for (int n=0;n<n_cols;n++) {
    si->setInteger(n);
  }

  //write the MPS file to a file called example.mps
  si->writeMps("example");

  // Solve the (relaxation of the) problem
  si->initialSolve();
  //si->branchAndBound();

  // Check the solution
  if ( si->isProvenOptimal() ) { 
     std::cout << "Found optimal solution!" << std::endl; 
     std::cout << "Objective value is " << si->getObjValue() << std::endl;

     int n = si->getNumCols();
     const double* solution = si->getColSolution();

     // We can then print the solution or could examine it.
     for( int i = 0; i < n; ++i )
        std::cout << si->getColName(i) << " = " << solution[i] << std::endl;

  } else {
     std::cout << "Didn't find optimal solution." << std::endl;
     // Could then check other status functions.
  }

  delete[] row_ub;
  delete[] row_lb;
  delete[] objective;
  delete[] col_ub;
  delete[] col_lb;

  return 0;
}
