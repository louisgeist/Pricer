#include <iostream>
#include "cdf.h"
#include "european_call.h"
#include "european_put.h"
#include "asian_call.h"
#include "asian_put.h"
#include "asset.h"
#include <iomanip>
#include "american_call.h"
#include "european_put.h"
using namespace std;

int main()
{	std::cout << "WELCOME to our pricer !\n";
	cout << "Let's introce you some features of our program\n :";
	/// Default values for options and underlying
	double K = 100.0;
	double S = 100.0;
	double T = 1;
	double sigma = 0.30;

	// Assets construction
	asset my_asset;
	my_asset.set_SpotPrice(S);
	my_asset.set_Volatility(sigma);
	my_asset.set_CurrentTime(0.0) ;

	asset my_asset_no_div(my_asset);

	my_asset.get_alias_Dividends().set_Type(1);
	my_asset.get_alias_Dividends().set_Rate(0.0125);
	my_asset.get_alias_Dividends().set_Periods(0.25);
	my_asset.get_alias_Dividends().set_Next(0.1666);
	my_asset.set_r(0.05);

	// Options construction
	asset* ptr_underlying = &my_asset;

	european_call my_european_call(ptr_underlying,K,T);
	cout << my_european_call << endl;
	my_european_call.replication();

	asian_put my_asian_put(ptr_underlying, K, T);
	cout << my_asian_put << endl;
	my_asian_put.replication();

	american_call my_american_call(ptr_underlying, K, T);
	cout << my_american_call << endl;


	//---------- Initialization of an option and its underlying by the user ----------
	cout << "\nCreate your own option :";
	european_call euro;
	cin >> euro;

	std::cout << "\nEnter the risk free rate of your market : ";
	double risk_free_rate;
	std::cin >> risk_free_rate;
	my_asset.set_r(0.05);


	cout << euro << endl;
	
	// To illustrate the management of a display of an option created by default
	european_put default_put;
	cout << default_put << endl;

	

	return 0;
}