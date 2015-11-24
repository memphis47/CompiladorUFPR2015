program exemplo75 (input, output);
var m,n :  integer;
    l,s,k :  integer;
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
end.

