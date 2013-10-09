
from numpy import *
from datetime import datetime

mat_a = mat(random.rand(3000,3000))
mat_b = mat(random.rand(3000,3000))

start = datetime.now()

mat_c = mat_a*mat_b

end = datetime.now()
elapsed = end - start
print("time elapsed: " + str(elapsed))
