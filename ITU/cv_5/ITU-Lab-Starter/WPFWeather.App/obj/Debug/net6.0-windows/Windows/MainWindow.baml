   M S B A M L   `   `   `  ����I  EWPFWeather.App, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullE'clr-namespace:WPFWeather.App.ConvertersWPFWeather.App.Converters  5      E'clr-namespace:WPFWeather.App.ViewModelsWPFWeather.App.ViewModels  5      '    !WPFWeather.App.Windows.MainWindow   5      W SPresentationCore, Version=6.0.2.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=6.0.2.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35e aSystem.Windows.Controls.Ribbon, Version=6.0.2.0, Culture=neutral, PublicKeyToken=b77a5c561934e089R NWindowsBase, Version=6.0.2.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     R NSystem.Xaml, Version=6.0.2.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5      @d2http://schemas.microsoft.com/expression/blend/2008     5      Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5      >
converters'clr-namespace:WPFWeather.App.Converters     5      >
viewModels'clr-namespace:WPFWeather.App.ViewModels     5      _ [Microsoft.Xaml.Behaviors, Version=1.1.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a3b+http://schemas.microsoft.com/xaml/behaviors  5        � Title$  WPF Weather Demo��5      $	��312��6   $	��500��6+    � 	Resources 5      9   3WPFWeather.App.Converters.BoolToVisibilityConverter   boolToVisibilityConverter��%   &	          5   
   6/   5      .��5      �  � RowDefinitions 5   
   �� 5      $	��17* �6   �� 5      $	��12* �6   �� 5      $	��12* �6   �� 5      $��7* �6   �� 5      $	��10* �6   �� 5      $	��10* �6   .+�5   
   �� 
���   �$��Stretch=�6!   $��5��6=    J� OpacityMask 5      �� 5        �� 
StartPoint$	 0,1&�6&    �� EndPoint$	 0,0&�67   .��5!      �  � Color$ Transparent��6#    � Offset$ 1Q�67   � 5"      $ Black��6#   $
 0.65Q�61   � 5#      $ Black��6#   $	 0.2Q�61   � 5$      $ Transparent��6#   $ 0Q�67   5%      5'      �� 5*   
   $	��100��60   $	��100��6=   $��Center=�5+      $��Center=�6-   $��0��5,      ��5*      �� *Weather.Icon+5,      �� 5/   
   -   5         ��Name$ CityComboBox��5/      $��Stretch=�51      $��Center=�61   $��Center=�52      $��Center=�67   $��30(�53      $
��Bold"�6!   $��1��54      $��	15,0,15,0q�6    $��0,5,0,5q�63   �� 57   
   
�������$��Stretch=�59      	 �� TextAlignment$	 Center=�62   $��Center=�6I   $��50(�5:      $
��Bold"�6"   $��2��5;      ��57      �� *Weather.Temperature+
 �� StringFormat$
 {0:0}°��5;      �� 5>   
    �� Orientation$ 
Horizontal=�6   $��Center=�6.   $��3��6K   .+�5?      � $��30��6   $��30��6!   
������� � LayoutTransform 5@      �� 5A       �� Angle 6&   �� *Weather.WindDirection+ � Points 5D      8�  8� X$ 15Q�6    8� Y$ 0Q�6#   8� 5E      $ 10Q�6   $ 30Q�6#   8� 5F      $ 20Q�6   $ 30Q�6#   5H      �� 5I      
�������$��20(�5J   +   $��30��69   ��5I      �� *Weather.WindSpeed+$
 {0:0.00} m/s��5J   9   5K      �� 5Q   
   
�������$��Stretch=�5S      $	 Center=�62   $��Center=�6I   $��50(�5T      $
��Bold"�6"   $��4��5U      ��5Q      �� *Weather.Description+5U      5V      5W      