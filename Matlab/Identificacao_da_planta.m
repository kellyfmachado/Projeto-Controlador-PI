%IMPORTAÇÃO DE DADOS
dados = load("Dados_SCR.txt");

t = dados(:,1);
tem = dados(:,2);
dut = dados(:,3);

t = t/1000;

% GRÁFICOS A PARTIR DOS DADOS
figure(1);
plot(t, tem, 'b');
xlabel('Tempo (s)');
ylabel('Temperatura (°C)');
title('Gráfico da Temperatura');

figure (2);
plot(t, tem, 'b', t, dut, 'r');
xlabel('Tempo (s)');
ylabel('Temperatura (°C) e Duty Cycle (%)');
legend('Saída','Entrada');
title('Relação entre temperatura e duty cycle do cooler');

% SELEÇÃO DA PARTE DE ATUAÇÃO DO COOLER
t = t(1750:2084,1);
tem = tem(1750:2084,1);
dut = dut(1750:2084,1);

figure(3);
plot(t, tem, 'b');
xlabel('Tempo (s)');
ylabel('Temperatura (°C)');
title('Temperatura com Atuação do Cooler');

% MUDANÇA DE ESCALA DOS GRÁFICOS
dut(39:end) = 1;
tem = rescale(tem,"InputMin",32,"InputMax",40);

figure(4);
plot(t, tem,'b', t, dut, 'k');
xlabel('Tempo (s)');
ylabel('Temperatura (°C) e Duty Cycle');
legend('Saída','Entrada');
title('Mudança de Escala da Temperatura e Duty Cycle');

% OFFSET PARA O GRÁFICO DE TEMPERATURA COMEÇAR EM ZERO
media = mean(tem(1:38));
tem = tem - media;

% GRÁFICO COM OFFSET
figure(5);
plot(t, tem,'b', t, dut, 'k');
xlabel('Tempo (s)');
ylabel('Temperatura (°C) e Duty Cycle');
legend('Saída','Entrada');
title('Temperatura (com offset) e Duty Cycle');

% INTERVALO DE AMOSTRAGEM
dT = t(2)-t(1); 		

% IDENTIFICAÇÃO DO SISTEMA
dados = iddata(tem, dut, dT);
np = 1;
nz = 0;
FT = tfest(dados,np,nz,NaN)
GT = tf(FT.num,FT.den,'InputDelay',FT.IODelay)

% SIMULAÇÃO DA FT IDENTIFICADA
y_sim = lsim(GT,dut,t);

% PLOTAGEM DO GRÁFICO DA FUNÇÃO IDENTIFICADA
figure(6)
plot(t, dut,'k',t,tem,'b',t,y_sim,'r')
title('Função identificada');
legend('Entrada','Saída','Função identificada');
xlabel('Tempo (s)');
ylabel('Temperatura (°C)');

% MUDANÇA PARA O OFFSET ORIGINAL E PLOTAGEM DO GRÁFICO DA FUNÇÃO
tem = tem + media;
y_sim = y_sim + media;

figure(7)
plot(t,tem,'b',t,y_sim,'r')
title('Função identificada');
legend('Temperatura','Função identificada');
xlabel('Tempo (s)');
ylabel('Temperatura (°C)');


