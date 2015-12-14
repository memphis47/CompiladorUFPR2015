program testebool2(input, output);

var b : boolean;
   a  : integer;

procedure p(var c : boolean);
var d : boolean;
begin
   d := false;
   if (1>2) then 
      d:=true;
   c:=d;
end;

begin
   p(b);
   write(b);
end.