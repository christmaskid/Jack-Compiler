#include "Variable.h"

Variable::Variable()
{
    kind=fail;
    type="none";
    name="";
    num=-1;
}

void Variable::setData(word k,string t,string na,int n){
    kind=k; type=t; name=na; num=n;
}
