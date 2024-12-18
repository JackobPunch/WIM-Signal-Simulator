% Program do generowania histogramów masy całkowitej i nacisków osi
% z obliczeniem błędu statystycznego liczbowego i procentowego

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

% Obliczenia statystyczne dla masy całkowitej
[mean_weight, error_weight, error_percent_weight] = calculate_error(weight_total);

% Wyświetlenie histogramu masy całkowitej
figure('Name', 'Histogram masy całkowitej', 'NumberTitle', 'off');
plot_histogram(weight_total, 'Histogram zarejestrowanej masy całkowitej', 'Masa [kg]');
display_stats(mean_weight, error_weight, error_percent_weight, 'Masa całkowita');

% Histogram i obliczenia dla każdej osi
for i = 1:num_axes
    [mean_axis, error_axis, error_percent_axis] = calculate_error(axes_pressures(:, i));
    
    figure('Name', ['Histogram masy osi ', num2str(i)], 'NumberTitle', 'off');
    plot_histogram(axes_pressures(:, i), ...
        ['Histogram odczytanej masy osi ', num2str(i)], 'Masa [kg]');
    display_stats(mean_axis, error_axis, error_percent_axis, ['Oś ', num2str(i)]);
end

function plot_histogram(data, title_text, xlabel_text)
    unique_values = unique(data); % Unikalne wartości
    num_bins = length(unique_values); % Liczba binów = liczba unikalnych wartości
    h = histogram(data, 'BinEdges', linspace(min(data), max(data), num_bins + 1), ...
        'EdgeColor', 'black', 'FaceColor', [0, 0.5, 1]);
    
    ymax = max(h.Values); % Maksymalna wartość wysokości słupków
    ylim([0, ymax + 0.1]); % Ustawienie zakresu osi y
    
    title(title_text);
    xlabel(xlabel_text);
    ylabel('Ilość odczytów');
    grid on;
end

% Definicja funkcji: obliczenie błędu statystycznego
function [mean_value, error_value, error_percent] = calculate_error(data)
    mean_value = mean(data);                % Średnia arytmetyczna
    std_dev = std(data);                    % Odchylenie standardowe
    N = length(data);                       % Liczba pomiarów
    error_value = std_dev / sqrt(N);        % Błąd statystyczny
    error_percent = (error_value / mean_value) * 100; % Błąd procentowy
end

% Definicja funkcji: wyświetlenie statystyk na wykresie
function display_stats(mean_val, error_val, error_percent, label_text)
    fprintf('%s: Średnia = %.2f, Błąd = %.4f, Błąd %% = %.2f%%\n', ...
        label_text, mean_val, error_val, error_percent);
    %annotation('textbox', [0.15, 0.8, 0.1, 0.1], 'String', ...
        %sprintf('Średnia = %.2f\nBłąd = %.4f\nBłąd %% = %.2f%%', ...
       % mean_val, error_val, error_percent), ...
       % 'FitBoxToText', 'on', 'BackgroundColor', 'white');
end
