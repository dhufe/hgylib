clear 
close all

load ../build/test.mat

[x,y] = meshgrid ( recparm.Start(2):recparm.Scale(2):recparm.Sizes(2)-recparm.Scale(2),recparm.Start(1):recparm.Scale(1):recparm.Sizes(1)-recparm.Scale(1) );

figure(1)
pcolor ( x,y, HGYCScan_0 )
colormap (jet) 
colorbar 
shading interp

figure(2)
pcolor ( x,y, HGYCScan_1 )
colormap (jet) 
colorbar 
shading interp

figure(3)
pcolor ( x,y, HGYDScan_2 )
colormap (jet) 
colorbar 
shading interp