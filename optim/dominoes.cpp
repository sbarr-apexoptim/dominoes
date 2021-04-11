#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <glpk.h>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

int main(void) {
  double large_val = 1e8;

  glp_prob *mip = glp_create_prob();
  glp_set_prob_name(mip, "dominoes");
  glp_set_obj_dir(mip, GLP_MIN);
  glp_term_out(GLP_ON);

  //int n_sets = 1;
  //int n_sets = 4;
  int n_sets = 48;
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

  int imax = 176;
  int jmax = 120;
  //int imax = 20;
  //int jmax = 22;
  //int imax = 10;
  //int jmax = 11;
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

  int n_vars = (imax-1) * jmax * n_types + // Whether tile k is on vertical edge i,j
               imax * (jmax-1) * n_types + // Whether tile k is on horizontal edge i,j
               imax * jmax + // Number of dots on cell i,j
               imax * jmax + // Difference between number of dots and brightness in cell
               imax * jmax;// + // t1 i,j

  int n_constraints = imax * jmax + // Number of tiles on cell i,j equal to sum of tiles on adjacent edges
                      imax * jmax + // Number of dots on cell i,j equal to sum of tile dots on adjacent edges
                      imax * jmax + // Difference between number of dots and brightness in cell
//                      10 + // These are the tiles which are symmetric, so the flipped version is the same as the unflipped.
//                      45 + // Optimizer can decide to place a flipped or unflipped tile on an edge, but they are really the same tile, so the sum of those tiles should equal the number of sets of dominoes
                      imax * jmax + //  Absolute values
                      imax * jmax; //  Absolute values
                      

  glp_add_cols(mip, n_vars);
  // Count of tiles on cell i,j
  cnt = 1;

  for (int i=0;i<imax-1;i++) {
    for (int j=0;j<jmax;j++) {
      for(int k=0;k<n_types;k++){
        string name = "Tv_i-"+std::to_string(i)+"_j-"+std::to_string(j)+"_k-"+std::to_string(k);
        glp_set_col_name(mip, cnt, name.c_str());
        glp_set_col_bnds(mip, cnt, GLP_DB, 0.0, 1.0);
        glp_set_obj_coef(mip, cnt, 0.0);
        glp_set_col_kind(mip, cnt, GLP_BV);
        cnt += 1;
      }
    }
  }

  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax-1;j++) {
      for(int k=0;k<n_types;k++){
        string name = "Th_i-"+std::to_string(i)+"_j-"+std::to_string(j)+"_k-"+std::to_string(k);
        glp_set_col_name(mip, cnt, name.c_str());
        glp_set_col_bnds(mip, cnt, GLP_DB, 0.0, 1.0);
        glp_set_obj_coef(mip, cnt, 0.0);
        glp_set_col_kind(mip, cnt, GLP_BV);
        cnt += 1;
      }
    }
  }

  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax;j++) {
      string name = "v_i-"+std::to_string(i)+"_j-"+std::to_string(j);
      glp_set_col_name(mip, cnt, name.c_str());
      glp_set_col_bnds(mip, cnt, GLP_DB, 0.0, 9.0);
      glp_set_obj_coef(mip, cnt, 0.0);
      glp_set_col_kind(mip, cnt, GLP_IV);
      cnt += 1;
    }
  }

  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax;j++) {
      string name = "x_i-"+std::to_string(i)+"_j-"+std::to_string(j);
      glp_set_col_name(mip, cnt, name.c_str());
      glp_set_col_bnds(mip, cnt, GLP_DB, -9.0, 9.0);
      glp_set_obj_coef(mip, cnt, 0.0);
      glp_set_col_kind(mip, cnt, GLP_IV);
      cnt += 1;
    }
  }

  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax;j++) {
      string name = "t1_i-"+std::to_string(i)+"_j-"+std::to_string(j);
      glp_set_col_name(mip, cnt, name.c_str());
      glp_set_col_bnds(mip, cnt, GLP_DB, 0.0, 9.0);
      glp_set_obj_coef(mip, cnt, 1.0);
      glp_set_col_kind(mip, cnt, GLP_IV);
      cnt += 1;
    }
  }

  glp_add_rows(mip, n_constraints);

  int non_zeros = 0;

  cnt = 1;

  // Tiles occupying cell i,j from adjacent edges
  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax;j++) {
      string name = "edge_i-"+std::to_string(i)+"_j-"+std::to_string(j);
      glp_set_row_name(mip, cnt, name.c_str());
      glp_set_row_bnds(mip, cnt, GLP_FX, 1.0, 0);
      cnt += 1;
    }
  }
  non_zeros += ((2*(imax-1)*jmax*n_types) + (2*imax*(jmax-1)*n_types));

  // Number of dots on cell i,j equal to sum of tile dots on adjacent edges
  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax;j++) {
      string name = "number-of-dots_i-"+std::to_string(i)+"_j-"+std::to_string(j);
      glp_set_row_name(mip, cnt, name.c_str());
      glp_set_row_bnds(mip, cnt, GLP_FX, 0.0, 0);
      cnt += 1;
    }
  }
  non_zeros += (imax*jmax + (2*(imax-1)*jmax*n_types) + (2*imax*(jmax-1)*n_types));

  // Difference between number of dots and brightness in cell
  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax;j++) {
      string name = "brightness_i-"+std::to_string(i)+"_j-"+std::to_string(j);
      glp_set_row_name(mip, cnt, name.c_str());
      glp_set_row_bnds(mip, cnt, GLP_FX, brightness[i][j], 0);
      cnt += 1;
    }
  }
  non_zeros += (2*imax*jmax);

//  // Number of symmetric tiles used of each type equal to number of sets
//  for (int n=0;n<10;n++) {
//    string name = "symmetric_n-"+std::to_string(n);
//    glp_set_row_name(mip, cnt, name.c_str());
//    //glp_set_row_bnds(mip, cnt, GLP_FX, n_sets, 0);
//    glp_set_row_bnds(mip, cnt, GLP_UP, 0, n_sets);
//    cnt += 1;
//  }
//  non_zeros += 10*(((imax-1)*jmax) + (imax*(jmax-1)));
//
//  // Number of non-symmetric tiles used of each type equal to number of sets
//  for (int n=10;n<55;n++) {
//    string name = "nonsymmetric_n-"+std::to_string(n);
//    glp_set_row_name(mip, cnt, name.c_str());
//    //glp_set_row_bnds(mip, cnt, GLP_FX, n_sets, 0);
//    glp_set_row_bnds(mip, cnt, GLP_UP, 0, n_sets);
//    cnt += 1;
//  }
//  non_zeros += 45*(2*((imax-1)*jmax) + (2*imax*(jmax-1)));

  // Absolute values
  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax;j++) {
      string name = "absolute-value1_i-"+std::to_string(i)+"_j-"+std::to_string(j);
      glp_set_row_name(mip, cnt, name.c_str());
      glp_set_row_bnds(mip, cnt, GLP_UP, 0.0, 0.0);
      cnt += 1;
    }
  }
  non_zeros += (2*imax*jmax);
  // Absolute values
  for (int i=0;i<imax;i++) {
    for (int j=0;j<jmax;j++) {
      string name = "absolute-value2_i-"+std::to_string(i)+"_j-"+std::to_string(j);
      glp_set_row_name(mip, cnt, name.c_str());
      glp_set_row_bnds(mip, cnt, GLP_UP, 0.0, 0.0);
      cnt += 1;
    }
  }
  non_zeros += (2*imax*jmax);

  int *ia, *ja;
  double *ar;
  ia = new int[non_zeros+1];
  ja = new int[non_zeros+1];
  ar = new double[non_zeros+1];
  cnt = 1;
  int n=1, nc=1;

  int cnt1 = cnt;
  // Cells on each tile equal to sum of those on adjacent edges
  for(int i=0;i<imax;i++) {
    for(int j=0;j<jmax;j++) {
      n = 1;
      // Cycle through vertical edge variables
      for(int i2=0;i2<imax-1;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          for(int k2=0;k2<n_types;k2++) {
            if(i == i2 && j == j2) {
              ia[cnt] = nc;
              ja[cnt] = n;
              ar[cnt] = 1.0;
              cnt++;
            } else if(i == i2+1 && j == j2) {
              ia[cnt] = nc;
              ja[cnt] = n;
              ar[cnt] = 1.0;
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
              ia[cnt] = nc;
              ja[cnt] = n;
              ar[cnt] = 1.0;
              cnt++;
            } else if(i == i2 && j == j2+1) {
              ia[cnt] = nc;
              ja[cnt] = n;
              ar[cnt] = 1.0;
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
      nc++;
    }
  }
  //cout << cnt - cnt1 << endl;

  // Number of dots constraint
  for(int i=0;i<imax;i++) {
    for(int j=0;j<jmax;j++) {
      n = 1;
      // Cycle through vertical edge variables
      for(int i2=0;i2<imax-1;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          for(int k2=0;k2<n_types;k2++) {
            if(i == i2 && j == j2) {
              ia[cnt] = nc;
              ja[cnt] = n;
              ar[cnt] = phi[k2][0];
              cnt++;
            } else if(i == i2+1 && j == j2) {
              ia[cnt] = nc;
              ja[cnt] = n;
              ar[cnt] = phi[k2][1];
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
              ia[cnt] = nc;
              ja[cnt] = n;
              ar[cnt] = phi[k2][0];
              cnt++;
            } else if(i == i2 && j == j2+1) {
              ia[cnt] = nc;
              ja[cnt] = n;
              ar[cnt] = phi[k2][1];
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
            ia[cnt] = nc;
            ja[cnt] = n;
            ar[cnt] = -1.0;
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
      nc++;
    }
  }

  // Difference in brightness contraint
  for(int i=0;i<imax;i++) {
    for(int j=0;j<jmax;j++) {
      n = 1;
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
            ia[cnt] = nc;
            ja[cnt] = n;
            ar[cnt] = 1.0;
            cnt++;
          }
          n++;
        }
      }
      // Cycle through x variables
      for(int i2=0;i2<imax;i2++) {
        for(int j2=0;j2<jmax;j2++) {
          if(i == i2 && j == j2) {
            ia[cnt] = nc;
            ja[cnt] = n;
            ar[cnt] = -1.0;
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
      nc++;
    }
  }

/*
  // Symmetric tiles constraint
  for(int k=0;k<10;k++) {
    n = 1;
    // Cycle through vertical edge variables
    for(int i2=0;i2<imax-1;i2++) {
      for(int j2=0;j2<jmax;j2++) {
        for(int k2=0;k2<n_types;k2++) {
          if(k == k2) {
            ia[cnt] = nc;
            ja[cnt] = n;
            ar[cnt] = 1.0;
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
            ia[cnt] = nc;
            ja[cnt] = n;
            ar[cnt] = 1.0;
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
    nc++;
  }

  // Non-symmetric tiles constraint
  for(int k=10;k<55;k++) {
    n = 1;
    // Cycle through vertical edge variables
    for(int i2=0;i2<imax-1;i2++) {
      for(int j2=0;j2<jmax;j2++) {
        for(int k2=0;k2<n_types;k2++) {
          if(k2 > 9) {
            if(k == k2 || k == k2-45) {
              ia[cnt] = nc;
              ja[cnt] = n;
              ar[cnt] = 1.0;
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
              ia[cnt] = nc;
              ja[cnt] = n;
              ar[cnt] = 1.0;
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
    nc++;
  }
*/

  // Absolute value constraint
  for(int i=0;i<imax;i++) {
    for(int j=0;j<jmax;j++) {
      n = 1;
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
            ia[cnt] = nc;
            ja[cnt] = n;
            ar[cnt] = 1.0;
            cnt++;
          }
          n++;
        }
      }
      // Cycle through t1 variables
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
      nc++;
    }
  }

  // Absolute value 2 constraint
  for(int i=0;i<imax;i++) {
    for(int j=0;j<jmax;j++) {
      n = 1;
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
            ia[cnt] = nc;
            ja[cnt] = n;
            ar[cnt] = -1.0;
            cnt++;
          }
          n++;
        }
      }
      // Cycle through t1 variables
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
      nc++;
    }
  }

  cout << "load" << endl;
  glp_load_matrix(mip, non_zeros, ia, ja, ar);
  cout << "Done" << endl;

  glp_iocp parm;
  glp_init_iocp(&parm);
  // Write out lp formulation to file
  glp_cpxcp cpparm;
  glp_init_cpxcp(&cpparm);
  //glp_write_lp(mip, &cpparm, "out.lp");
  //glp_write_mps(mip, GLP_MPS_FILE, NULL, "dominoes.mps");
  //glp_write_mps(mip, GLP_MPS_FILE, NULL, "dominoes.mps");
  parm.presolve = GLP_ON;
  int err = glp_intopt(mip, &parm);

  cout << glp_mip_obj_val(mip) << endl;

  cnt = 1;
  cout << "Horizontal" << endl;
  for(int i=0;i<imax-1;i++) {
    for(int j=0;j<jmax;j++) {
      for(int k=0;k<n_types;k++) {
        if(glp_mip_col_val(mip, cnt) == 1) {
          printf("i:%d, j:%d, left:%d, right:%d\n",i,j,phi[k][0],phi[k][1]);
        }
        cnt++;
      }
    }
  }
  cout << "Vertical" << endl;
  for(int i=0;i<imax;i++) {
    for(int j=0;j<jmax-1;j++) {
      for(int k=0;k<n_types;k++) {
        if(glp_mip_col_val(mip, cnt) == 1) {
          printf("i:%d, j:%d, top:%d, bottom:%d\n",i,j,phi[k][0],phi[k][1]);
        }
        cnt++;
      }
    }
  }

  delete [] ia;
  delete [] ja;
  delete [] ar;
  ia = NULL;
  ja = NULL;
  ar = NULL;

  glp_delete_prob(mip);
  return 0;
}
