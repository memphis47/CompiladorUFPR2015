program exemplo75 (input, output);
var m,n :  integer;
    procedure p(var k:integer; s:integer);
    var x,z : integer;
    begin
    	    k := 6;
    	    x := 5;
    	    write(k);
    	    write(x);
    		write(m);
    end;
    function f(z : integer): integer;
    begin
        if z < 0 then 
            write(z)
        else 
            f:=f(z-2)*z+1;
        write(z);
    end;
begin
	m := 2;
	n := 0;
	p(m,n);
	f(m);
end.
    