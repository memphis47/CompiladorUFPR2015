program exemplo75 (input, output);
var m,n :  integer;
    l,s,k :  boolean;
    f,a,v :  integer;
    c,aba,mama :  integer;
    mia,oh,mama :  boolean;
    
begin
	m := 0;
	l := 0;
	k := 1;
	m := m+k+l+4;
	write(l);
	if(m<k)then
	begin
		if(k <> 0) then
			write(k);
		else
			write(m);
	end;
	else
	begin
		if(k = 0) then
			write(k);
		else
			write(m);
	end;
	while (m>k) do
	begin
		if(k = 0) then
			write(k);
		else
			write(m);
		k:=k+1;
	end;

end.

