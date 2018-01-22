import math
import check


class Budget:
    '''Fields: month(Str), withdrawals (listof Nat), deposits (listof Nat),
          requires: month has format MONXX where:
          - MON is one of JAN,FEB,MAR,APR,MAY,JUN,JUL,AUG,SEP,OCT,NOV,DEC
          - XX is a 2-digit year starting from 00
          (e.g.MAR00 represents March 2000)'''  
    
    def __init__(self,mnth,low,lod):
        self.month = mnth
        self.withdrawals = low
        self.deposits = lod
        
    def __repr__(self):
        s = "Budget for the month of {0}:\nWithdrawals:{1}\nDeposits:{2}\nNet:{3}"
        return s.format(self.month,
                        self.withdrawals,
                        self.deposits,
                        sum(self.deposits) - sum(self.withdrawals))
    
    def __eq__(self, other):
        return type(other) == type(self) and self.month == other.month and \
               self.withdrawals == other.withdrawals and self.deposits == other.deposits  


## find(month,lob) will find the Budget of the month in lob and produces a list which contains
## it's corresponding withdrawals and deposits list
## find: Str (listof Budget) -> (listof (listof Budget))
## requires:
## lob is not an empty list
## examples:
## budgets = [Budget("MAR17",[],[10,10]),Budget("FEB17",[5,22],[30,10])]
## find('MAR17',budgets) => [[],[10,10]]
## find('FEB18',budgets) => [[5,22],[30,10]]

        
def find(month,lob):
    for b in lob:
        if b.month == month:
            return [b.withdrawals,b.deposits]

budgets = [Budget("MAR17",[],[10,10]),Budget("FEB17",[5,22],[30,10])]
check.expect('T1',find('MAR17',budgets),[[],[10,10]])
check.expect('T2',find('FEB17',budgets),[[5,22],[30,10]])

        

## best_and_worst(lob) produces a dictionary which has four pair of keys and values
## they are the months where the maximum deposits occurs, the months which the 
## maximum withdrawals occurs. Another two is the value of the maximum deposit and
## the value of the maximum withdrawal.
## best_and_worst: (listof Budget) -> (dictof Str (anyof (listof Str) Nat))
## requires:
## there is at least one withdrawal and one deposit in	the entire list
## no Budget object will represent the exact same month	and year
## examples:
## best_and_worst([Budget("MAR17",[],[10,10]),Budget("FEB17",[5,22],[30,10])]) =>\
## {'D_month:['FEB17'],'W_month:['FEB17'],'maxD':30,'maxW':22}
## best_and_worst([Budget("MAR17",[20,30],[10,10]),Budget("FEB17",[5,22],[30,10])]) =>\
## {'D_month:['FEB17'],'W_month:['MAR17'],'maxD':30,'maxW':30}

def best_and_worst(lob):
    maxD = 0
    maxW = 0
    D_months=[]
    W_months=[]
    result = {}
        
        
    for b in lob:
            
        for w in b.withdrawals:
            if w >= maxW :
                maxW = w
                if not b.month in W_months:
                    W_months.append (b.month)
            
        for d in b.deposits:
            if d >= maxD :
                maxD = d
                if not b.month in D_months:
                    D_months.append (b.month)
                
    D_months=list(filter(lambda x: maxD in find(x,lob)[1] , D_months))
                         
    W_months=list(filter(lambda x: maxW in find(x,lob)[0], W_months))               
        
    result['maxD']= maxD
    result['maxW']= maxW
    result['D_months']= D_months
    result['W_months']= W_months
        
    return result
    
budgets = [Budget("MAR17",[],[10,10]),Budget("FEB17",[5,22],[30,10]),
           Budget("JAN17",[2,7,3,8],[5,10,20]),
           Budget("DEC16",[25],[]),Budget("NOV16",[5,10,10,5],[30,5])]

budgets1 = [Budget("MAR17",[5,22],[10,10]),Budget("FEB17",[5,22],[10,10]),
           Budget("JAN17",[5,22],[10,10]),
           Budget("DEC16",[5,22],[10,10]),Budget("NOV16",[5,22],[10,10])]

check.expect('T3',best_and_worst(budgets),
             {'D_months': ['FEB17', 'NOV16'], 
              'W_months': ['DEC16'], 'maxW': 25, 'maxD': 30})
check.expect('T4',best_and_worst(budgets1),
             {'D_months': ['MAR17','FEB17','JAN17','DEC16','NOV16'], 
              'W_months': ['MAR17','FEB17','JAN17','DEC16','NOV16'],
              'maxW': 22, 'maxD': 10})