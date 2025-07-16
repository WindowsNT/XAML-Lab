// 

const char* template_1 = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<e>
	<Design>
		<Grid>
			<Grid.RowDefinitions>
				<RowDefinition Height="Auto"/>
				<RowDefinition Height="1*"/>
			</Grid.RowDefinitions>
			<Grid.ColumnDefinitions>
				<ColumnDefinition Width="1*"/>
			</Grid.ColumnDefinitions>
			<MenuBar Menu="TemplateMenu1">
				<MenuBarItem Title="File">
					<MenuFlyoutItem Text="New"/>
					<MenuFlyoutItem Text="Open"/>
					<MenuFlyoutItem Text="Save"/>
					<MenuFlyoutItem Text="Save As..."/>
					<MenuFlyoutSeparator />
					<MenuFlyoutItem Text="Exit"/>
				</MenuBarItem>
				<MenuBarItem Title="Edit">
					<MenuFlyoutItem Text="Undo"/>
					<MenuFlyoutItem Text="Redo"/>
					<MenuFlyoutSeparator />
					<MenuFlyoutItem Text="Select All"/>
					<MenuFlyoutItem Text="Unselect All"/>
				</MenuBarItem>
				<MenuBarItem Title="View">
					<MenuFlyoutItem Text="Menu"/>
					<MenuFlyoutItem Text="Toolbar"/>
					<MenuFlyoutItem Text="Statusbar"/>
				</MenuBarItem>
				<MenuBarItem Title="Help">
					<MenuFlyoutItem Text="Help"/>
					<MenuFlyoutSeparator />
					<MenuFlyoutItem Text="About..."/>
				</MenuBarItem>
			</MenuBar>
			<ListView Grid.Row="1" Margin="10,10,10,10" HorizontalAligment="Left"/>
		</Grid>
	</Design>
	<Menus>
		<e n="TemplateMenu1">
			<Root n="Root">
				<e n="File">
					<e n="New"/>
					<e n="Open"/>
					<e n="Save"/>
					<e n="Save As..."/>
					<e sep="1" n="---"/>
					<e n="Exit"/>
				</e>
				<e n="Edit">
					<e n="Undo"/>
					<e n="Redo"/>
					<e sep="1" n="---"/>
					<e n="Select All"/>
					<e n="Unselect All"/>
				</e>
				<e n="View">
					<e n="Menu"/>
					<e n="Toolbar"/>
					<e n="Statusbar"/>
				</e>
				<e n="Help">
					<e n="Help"/>
					<e sep="1" n="---"/>
					<e n="About..."/>
				</e>
			</Root>
		</e>
	</Menus>
</e>
)";


const char* template_2 = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<e>
	<Design>
		<Grid>
			<Grid.RowDefinitions>
				<RowDefinition Height="Auto"/>
				<RowDefinition Height="1*"/>
			</Grid.RowDefinitions>
			<Grid.ColumnDefinitions>
				<ColumnDefinition Width="1*"/>
			</Grid.ColumnDefinitions>
			<MenuBar Menu="TemplateMenu2">
				<MenuBarItem Title="File">
					<MenuFlyoutItem Text="New"/>
					<MenuFlyoutItem Text="Open"/>
					<MenuFlyoutItem Text="Save"/>
					<MenuFlyoutItem Text="Save As..."/>
					<MenuFlyoutSeparator />
					<MenuFlyoutItem Text="Exit"/>
				</MenuBarItem>
				<MenuBarItem Title="Edit">
					<MenuFlyoutItem Text="Undo"/>
					<MenuFlyoutItem Text="Redo"/>
					<MenuFlyoutSeparator />
					<MenuFlyoutItem Text="Select All"/>
					<MenuFlyoutItem Text="Unselect All"/>
				</MenuBarItem>
				<MenuBarItem Title="View">
					<MenuFlyoutItem Text="Menu"/>
					<MenuFlyoutItem Text="Toolbar"/>
					<MenuFlyoutItem Text="Statusbar"/>
				</MenuBarItem>
				<MenuBarItem Title="Help">
					<MenuFlyoutItem Text="Help"/>
					<MenuFlyoutSeparator />
					<MenuFlyoutItem Text="About..."/>
				</MenuBarItem>
			</MenuBar>
			<RichEditBox Grid.Row="1" Margin="10,10,10,10" />
		</Grid>
	</Design>
	<Menus>
		<e n="TemplateMenu2">
			<Root n="Root">
				<e n="File">
					<e n="New"/>
					<e n="Open"/>
					<e n="Save"/>
					<e n="Save As..."/>
					<e sep="1" n="---"/>
					<e n="Exit"/>
				</e>
				<e n="Edit">
					<e n="Undo"/>
					<e n="Redo"/>
					<e sep="1" n="---"/>
					<e n="Select All"/>
					<e n="Unselect All"/>
				</e>
				<e n="View">
					<e n="Menu"/>
					<e n="Toolbar"/>
					<e n="Statusbar"/>
				</e>
				<e n="Help">
					<e n="Help"/>
					<e sep="1" n="---"/>
					<e n="About..."/>
				</e>
			</Root>
		</e>
	</Menus>
</e>
)";

const char* template_3 = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<e>
	<Design>
		<Grid>
			<Grid.RowDefinitions>
				<RowDefinition Height="Auto"/>
				<RowDefinition Height="1*"/>
			</Grid.RowDefinitions>
			<Grid.ColumnDefinitions>
				<ColumnDefinition Width="1*"/>
			</Grid.ColumnDefinitions>
			<WebView2 Grid.Row="1" Margin="10,10,10,10" HorizontalAligment="Left" VerticalAligment="Top" MinHeight="1080" MinWidth="1920" />
		</Grid>
	</Design>
	<Menus>
</e>
)";


const char* template_4 = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<e>
	<Design>
		<NavigationView PaneDisplayMode="LeftCompact" IsSettingsVisible="True" IsPaneOpen="True" IsBackButtonVisible="Auto" Page="Page1">
			<NavigationViewItem Icon="Pictures" Content="Images"/>
		</NavigationView>
	</Design>
	<Pages>
		<e n="Page1">
			<Grid>
				<Grid.RowDefinitions>
					<RowDefinition Height="1*"/>
				</Grid.RowDefinitions>
				<Grid.ColumnDefinitions>
					<ColumnDefinition Width="2*"/>
					<ColumnDefinition Width="1*"/>
				</Grid.ColumnDefinitions>
				<FlipView />
				<ListView Grid.Column="1"/>
			</Grid>
		</e>
	</Pages>
</e>
)";

const char* template_5 = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<e>
	<Design>
<StackPanel Margin="10,10,10,10">
	<TextBlock Text="Downloading: file.mp4" Margin="10,10,10,10"/>
	<ProgressBar IsIndeterminate="True" Margin="10,10,10,10" Minimum="0" Maximum="100" Value="0"/>
	<StackPanel Orientation="Horizontal" Margin="10,10,10,10">
		<Button Content="Pause" Margin="10,10,10,10"/>
		<Button Content="Cancel" Margin="10,10,10,10,"/>
	</StackPanel>
</StackPanel>
</Design>
</e>
)";

const char* template_6 = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<e>
	<Design>
		<Grid>
			<Grid.RowDefinitions>
				<RowDefinition Height="1*"/>
				<RowDefinition Height="1*"/>
			</Grid.RowDefinitions>
			<Grid.ColumnDefinitions>
				<ColumnDefinition Width="1*"/>
				<ColumnDefinition Width="1*"/>
			</Grid.ColumnDefinitions>
			<Button Click="WeClick" b___Content="a_button_name" b___FontSize="getFontSize"/>
		</Grid>
</Design>
</e>
)";


const char* template_7 = R"(<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<e>
	<Design>
		<Grid Name="GridMain">
			<Grid.RowDefinitions>
				<RowDefinition Height="Auto"/>
				<RowDefinition Height="Auto"/>
				<RowDefinition Height="1*"/>
				<RowDefinition Height="Auto"/>
				<RowDefinition Height="Auto"/>
			</Grid.RowDefinitions>
			<Grid.ColumnDefinitions>
				<ColumnDefinition Width="1*"/>
			</Grid.ColumnDefinitions>
			<Grid Margin="10,10,10,10" Name="GridTop">
				<Grid.RowDefinitions>
					<RowDefinition Height="1*"/>
				</Grid.RowDefinitions>
				<Grid.ColumnDefinitions>
					<ColumnDefinition Width="Auto"/>
					<ColumnDefinition Width="1*"/>
					<ColumnDefinition Width="Auto"/>
				</Grid.ColumnDefinitions>
				<BreadcrumbBar b___ItemsSource="BreadcrumbBarItems" ItemClicked="BIClicked" Grid.Column="1"/>
			</Grid>
			<Grid Grid.Row="1" Margin="10,10,10,10" Name="GridTools">
				<Grid.RowDefinitions>
					<RowDefinition Height="1*"/>
				</Grid.RowDefinitions>
				<Grid.ColumnDefinitions>
					<ColumnDefinition Width="Auto"/>
					<ColumnDefinition Width="Auto"/>
					<ColumnDefinition Width="Auto"/>
					<ColumnDefinition Width="1*"/>
				</Grid.ColumnDefinitions>
				<Button Content="UP" Margin="5,5,5,5" Click="ButtonUp"/>
				<Button Grid.Column="1" Content="NEW FOLDER" Margin="5,5,5,5" Click="ButtonNewFolder"/>
				<DropDownButton Grid.Column="2" Content="TOOLS" Menu="Menu1"/>
			</Grid>
			<Grid Grid.Row="2" Margin="10,10,10,10" Name="GridFiles">
				<Grid.RowDefinitions>
					<RowDefinition Height="1*"/>
				</Grid.RowDefinitions>
				<Grid.ColumnDefinitions>
					<ColumnDefinition Width="Auto"/>
					<ColumnDefinition Width="1*"/>
				</Grid.ColumnDefinitions>
				<TreeView Name="TreeBrowser" Margin="10,10,10,10" b___ItemsSource="TreeItems"/>
				<ListView Grid.Column="1" Name="ListFiles" HorizontalAlignment="Left" VerticalAlignment="Top" b___ItemsSource="FileItems"/>
			</Grid>
			<Grid Grid.Row="3" Margin="10,10,10,10" Name="GridInput">
				<Grid.RowDefinitions>
					<RowDefinition Height="1*"/>
				</Grid.RowDefinitions>
				<Grid.ColumnDefinitions>
					<ColumnDefinition Width="1*"/>
					<ColumnDefinition Width="Auto"/>
				</Grid.ColumnDefinitions>
				<TextBox Text="TextBox" HorizontalAlignment="Stretch" VerticalAlignment="Top" Margin="5,5,5,5" />
"				<ComboBox Grid.Column="1" MinWidth="200" HorizontalAlignment="Right" VerticalAlignment="Top" Margin="5,5,5,5" />
			</Grid>
			<Grid Grid.Row="4" Margin="10,10,10,10" Name="GridButtons">
				<Grid.RowDefinitions>
					<RowDefinition Height="1*"/>
				</Grid.RowDefinitions>
				<Grid.ColumnDefinitions>
					<ColumnDefinition Width="1*"/>
					<ColumnDefinition Width="Auto"/>
				</Grid.ColumnDefinitions>
				<StackPanel Orientation="Horizontal" HorizontalAlignment="Right">
					<Button Content="Open" Margin="10,10,10,10" Name="buttonOpen" Click="ButtonOK"/>
					<Button Content="Cancel" Margin="10,10,10,10" Name="buttonCancel" Click="ButtonCancel"/>
				</StackPanel>
			</Grid>
		</Grid>
	</Design>
	<Pages />
	<Menus>
		<e n="Menu1">
			<Root n="Root" n2="" sep="" g="" clicked="">
				<e n="View" g="" clicked="itClick" n2="" sep="">
					<e n="Grid" g="" clicked="buttonViewGrid" n2="" sep="" />				
					<e n="List" g="" clicked="buttonViewList" n2="" sep="" />				
				</e>				
			</Root>
		</e>

	</Menus>
</e>
)";

