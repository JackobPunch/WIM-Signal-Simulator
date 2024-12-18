% Program do generowania histogramów i analizy statystycznej
% Wczytanie danych z pliku CSV
data = readtable('2024_11_14_analiza100.csv', 'Delimiter', ';');

% Ekstrakcja masy całkowitej i nacisków osi
weight_total = data{:, 1}; % Masa całkowita
num_axes = 5; % Liczba osi

% Sumowanie nacisków lewej i prawej strony dla każdej osi
axes_pressures = zeros(height(data), num_axes);
for i = 1:num_axes
    axes_pressures(:, i) = data{:, 2*i} + data{:, 2*i + 1};
end

% Analiza masy całkowitej
analyze_data(weight_total, 'Masa całkowita');

% Analiza każdej osi
for i = 1:num_axes
    analyze_data(axes_pressures(:, i), ['Oś ', num2str(i)]);
end

% Funkcja analizy danych
function analyze_data(data, label_text)
    % Obliczenia statystyczne
    mean_value = mean(data);
    std_dev = std(data);
    variance = var(data);
    median_value = median(data);
    mode_value = mode(data); % Moda
    range_value = max(data) - min(data);
    error_stat = std_dev / sqrt(length(data)); % Błąd statystyczny średniej
    relative_error = (std_dev / mean_value) * 100; % Błąd względny
    
    % Wyświetlanie statystyk w konsoli
    fprintf('%s:\n', label_text);
    fprintf('Mediana = %.2f\nOdchylenie std = %.2f\nModa = %.2f\n', median_value, std_dev, mode_value);
    fprintf('Rozstęp = %.2f\nBłąd względny = %.2f%%\n\n', range_value, relative_error);
    
    % Tworzenie histogramu
figure('Name', ['Histogram ', label_text], 'NumberTitle', 'off');
h = histogram(data, 'BinEdges', linspace(min(data), max(data), 30), ...
    'EdgeColor', 'black', 'FaceColor', [0, 0.5, 1]);

% Ustawienia tytułu i etykiet osi
title(['Histogram ', label_text]);
xlabel('Masa [kg]');
ylabel('Ilość odczytów');
grid on;

% Pobranie wartości ymax i ustawienie ylim
ymax = max(h.Values); % Pobranie maksymalnej wartości z histogramu
ylim([0, ymax + 0.2]); % Ustawienie zakresu osi y

    % Dodanie linii mediany i wartości modalnej
    hold on;
    ylimits = ylim+1;
    %plot([median_value, median_value], ylimits, 'r--', 'LineWidth', 2); % Mediana
    %plot([mode_value, mode_value], ylimits, 'g--', 'LineWidth', 2); % Moda
    %legend('Dane', 'Mediana', 'Moda');
    hold off;

    % Dodanie statystyk na wykresie
    %annotation('textbox', [0.15, 0.7, 0.1, 0.1], 'String', ...
    %sprintf('Mediana = %.2f\nModa = %.2f\nOdch. std = %.2f', median_value, mode_value, std_dev), 'FitBoxToText', 'on', 'BackgroundColor', 'white');
    %annotation('textbox', [0.15, 0.7, 0.1, 0.1], 'String', sprintf('Mediana = %.2f', median_value), 'FitBoxToText', 'on', 'BackgroundColor', 'white');

end
