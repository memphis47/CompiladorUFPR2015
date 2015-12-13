program exemplo75 (input, output);
label 100,200;
var m,n : integer;
    procedure p(var k:integer; s:integer);
    var x,z : integer;
    begin
        100: k := 6;
    	     x := 5;
    	     write(k);
    	     write(x);
    		 write(m);
    		 goto 200;
    end;
    function f(z : integer): integer;
    begin
        if z < 0 then 
            write(z)
        else 
            f:=f(z-2)*z+1;
        write(z);
        goto 100;
    end;
begin
	 read(m,n);
     p(m,n);
	 n:= f(m);
200: write(m,n);
end.
    