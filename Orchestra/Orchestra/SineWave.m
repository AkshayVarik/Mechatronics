sinetable=[25,28,31,34,37,40,42,44,46,48,49,50,50,50,...
    50,49,48,46,44,42,40,37,34,31,25,22,19,16,13,10,...
    8,6,4,2,1,0,0,0,0,1,2,4,6,8,10,13,16,19,22,25];

OCR1A=1000;
OCR1B=0;

x_axis=linspace(1,50,50);

for i=1:50
    OCR1B(i)=(OCR1A*sinetable(i))/50;
	i=i+1;	
	if(i>50)
	{
		i==1;
	}
    end
end

st=size(sinetable);

plot(x_axis,OCR1B)