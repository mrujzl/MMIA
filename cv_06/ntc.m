clear all;
close all;

R5 = 10e3;
VCC = 3.3;
NTC = csvread('ntc.csv');

T = transpose(NTC(:,1));
R = transpose(NTC(:,2).*1e3);

ADC = (R./(R+R5))*1024;

P = polyfit(ADC,T,10);

ADC2 = 0:1023;
T2 = round(polyval(P,ADC2),1);

figure(1);
plot(ADC,T,'Marker','o');
xlabel('ADC value [-]');
ylabel('Temperature [°C]');
xlim([0,1100]);
ylim([-50,200]);
hold on;
plot(ADC2,T2,'Color','r');

dlmwrite('data.dlm', T2*10, ',');