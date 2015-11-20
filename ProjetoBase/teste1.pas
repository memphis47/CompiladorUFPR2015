program exemplo75 (input, output);
var m,n :  integer;
    l,s,k :  integer;
begin
	m := 0;
	n := 0;
	l := 0;
	s := 0;
	k := 0;
	m := m+k+l+4;
	k := l-s;
	s := s+(((k-l)+s)+l)-m;
	write(l);
	if(m>k)then
		write(s);
	else
		write(m);
	if(s < 0) then
	begin
		write(s);
		write(k);
	end
	else begin
		write(l);
	end
end.

