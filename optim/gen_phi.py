import numpy as np

symm_id = np.arange(10)
non_symm_id_1 = np.arange(10,55)
non_symm_id_2 = np.arange(55,100)

symm_val = np.zeros((10,2))
non_symm_val_1 = np.zeros((45,2))
non_symm_val_2 = np.zeros((45,2))

for i in range(0,10):
  symm_val[i][0] = i
  symm_val[i][1] = i

cnt = 0
for i in range(0,10):
  for j in range(0,10):
    if j > i:
      non_symm_val_1[cnt][0] = i
      non_symm_val_1[cnt][1] = j
      cnt += 1

cnt = 0
for i in range(0,10):
  for j in range(0,10):
    if j > i:
      non_symm_val_2[cnt][1] = i
      non_symm_val_2[cnt][0] = j
      cnt += 1

print(non_symm_val_2)
