program testebool3(input, output);

var b : boolean;

function f(c : boolean):boolean;
begin
if (c = true) 
  then
    f:=1>2
  else
  f:=1<2;

end;

begin
b:=f(true);
   write(b);
end.