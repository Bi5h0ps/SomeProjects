#############################################
import math
import check

#this function creates a child matrix since we have to 
#use the det of it to calculate the the det of the parent matrix
#example:
#   |1 2 3|                   |3 4|
# m=|2 3 4|   Mmatrix(m,1,1)= |4 5|
#   |3 4 5|
# i and j are the coordination you want to apply the function on
def Mmatrix(m,i,j):
    
    m = m[0:i-1] + m[i:]
    print(m)
    m1 = []
    for item in m:
        m1.append( item[:j-1]+item[j:] )
    
    return m1

#this function determins if its time to add the minor result or 
#subtract the minor result.
def cofactor(m,i,j):
    coe = (-1)**(i+j)
    minorij = det(Mmatrix(m,i,j))
    
    return coe*minorij

#this function calculates the determinate of a NxN matrix
def det(m):
    if len(m) == 1:
        return m[0][0]
    
    elif len(m) == 2:
        return m[0][0]*m[1][1] - m[0][1]*m[1][0]
    
    else:
        result = 0
        j=1
        for item in m[0]:
        
            result = result + item*cofactor(m,1,j)
            j = j+1
        return result












A = [[2,9,3,2],[4,0,0,6],[3,-1,1,2],[5,0,0,1]]
B = [[2,-2,1,1],[1,3,3,2],[1,0,9,1],[3,4,2,0]]
