using System.Collections.Generic;
using System.Windows;
using WPFWeather.App.Services;
using WPFWeather.App.ViewModels;

namespace WPFWeather.App.Windows
{
    public partial class MainWindow : Window
    {
        private readonly WeatherViewModel viewModel;

        private readonly List<string> cities = new List<string> { "Brno", "London", "Praha" };

        public MainWindow()
        {
            InitializeComponent();

            viewModel = new WeatherViewModel(new WeatherDiskService());
            DataContext = viewModel;

            CityComboBox.ItemsSource = cities;
            CityComboBox.SelectedItem = cities[0];

            viewModel.DownloadWeatherCommand.Execute(cities[0]);
        }

        private void CityChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
        {
            if (CityComboBox.SelectedItem is string selectedCity)
            {
                viewModel.DownloadWeatherCommand.Execute(selectedCity);
            }
        }
    }
}