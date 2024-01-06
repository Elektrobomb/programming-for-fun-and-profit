from bs4 import BeautifulSoup
from urllib.request import Request, urlopen
import numpy as np
import pandas as pd

def print_soup_contents(soup):
    print(soup.prettify())

def get_battery_price(soup):
    is_granada = soup.find("a", {"title": "Granada Batteries"}) is not None
    #print(is_granada)

    price_metas = soup.find_all("meta", {"itemprop": "price"})

    price = ""
    if is_granada:
        price = price_metas[1].get("content")
    else:
        price = price_metas[0].get("content")

    return float(price) 

def get_soup_from_url(url):
    req = Request(
        url=url,
        headers={'User-Agent': 'Mozilla/5.0'}
    )
    page = urlopen(req).read()
    html = page.decode("utf-8")
    soup = BeautifulSoup(html, "html.parser")
    return soup

def create_progress_bar(percentage):
    total_chars = 50
    completed_chars = int((percentage / 100) * total_chars)
    remaining_chars = total_chars - completed_chars

    progress_bar = "■" * completed_chars + "-" * remaining_chars
    return progress_bar

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    battery_types = ['AA', 'AAA', 'CR2032']
    battery_suppliers = ['Battery Station', 'Battery Group', 'Granada Batteries', 'Buy A Battery']
    battery_manufacturers = ['Procell Intense', 'Procell Constant', 'Energizer Industrial', 'Varta Industrial']

    manufacturers_and_suppliers = ['Procell Intense - Battery Station',
                                   'Procell Intense - Battery Group',
                                   'Procell Intense - Granada Batteries',
                                   'Procell Intense - Buy A Battery',
                                   'Procell Constant - Battery Station',
                                   'Procell Constant - Battery Group',
                                   'Procell Constant - Granada Batteries',
                                   'Procell Constant - Buy A Battery',
                                   'Energizer Industrial - Battery Station',
                                   'Energizer Industrial - Battery Group',
                                   'Energizer Industrial - Granada Batteries',
                                   'Energizer Industrial - Buy A Battery',
                                   'Varta Industrial - Battery Station',
                                   'Varta Industrial - Battery Group',
                                   'Varta Industrial - Granada Batteries',
                                   'Varta Industrial - Buy A Battery']

    links_df = pd.DataFrame(columns=battery_types, index=manufacturers_and_suppliers) #dataframe to store URL storepages

    test_url = 'https://www.batterystation.co.uk/duracell-procell-c-lr14-id1400-batteries-box-of-10/'


    ##### Insert the links to all store pages below #####
    #e.g.: links_df['AA']['Procell Intense - Battery Station'] = 'https://www.batterystation.co.uk/duracell-procell-c-lr14-id1400-batteries-box-of-10/'
    # sets the storepage link for Procell Intense AA batteries from Battery Station to https://www.batterystation.co.uk/duracell-procell-c-lr14-id1400-batteries-box-of-10/

    links_df['AA']['Procell Intense - Battery Station'] = test_url
    links_df['AA']['Procell Intense - Battery Group'] = test_url
    links_df['AA']['Procell Intense - Granada Batteries'] = test_url
    links_df['AA']['Procell Intense - Buy A Battery'] = test_url
    links_df['AA']['Procell Constant - Battery Station'] = test_url
    links_df['AA']['Procell Constant - Battery Group'] = test_url
    links_df['AA']['Procell Constant - Granada Batteries'] = test_url
    links_df['AA']['Procell Constant - Buy A Battery'] = test_url
    links_df['AA']['Energizer Industrial - Battery Station'] = test_url
    links_df['AA']['Energizer Industrial - Battery Group'] = test_url
    links_df['AA']['Energizer Industrial - Granada Batteries'] = test_url
    links_df['AA']['Energizer Industrial - Buy A Battery'] = test_url
    links_df['AA']['Varta Industrial - Battery Station'] = test_url
    links_df['AA']['Varta Industrial - Battery Group'] = test_url
    links_df['AA']['Varta Industrial - Granada Batteries'] = test_url
    links_df['AA']['Varta Industrial - Buy A Battery'] = test_url

    links_df['AAA']['Procell Intense - Battery Station'] = test_url
    links_df['AAA']['Procell Intense - Battery Group'] = test_url
    links_df['AAA']['Procell Intense - Granada Batteries'] = test_url
    links_df['AAA']['Procell Intense - Buy A Battery'] = test_url
    links_df['AAA']['Procell Constant - Battery Station'] = test_url
    links_df['AAA']['Procell Constant - Battery Group'] = test_url
    links_df['AAA']['Procell Constant - Granada Batteries'] = test_url
    links_df['AAA']['Procell Constant - Buy A Battery'] = test_url
    links_df['AAA']['Energizer Industrial - Battery Station'] = test_url
    links_df['AAA']['Energizer Industrial - Battery Group'] = test_url
    links_df['AAA']['Energizer Industrial - Granada Batteries'] = test_url
    links_df['AAA']['Energizer Industrial - Buy A Battery'] = test_url
    links_df['AAA']['Varta Industrial - Battery Station'] = test_url
    links_df['AAA']['Varta Industrial - Battery Group'] = test_url
    links_df['AAA']['Varta Industrial - Granada Batteries'] = test_url
    links_df['AAA']['Varta Industrial - Buy A Battery'] = test_url

    links_df['CR2032']['Procell Intense - Battery Station'] = test_url
    links_df['CR2032']['Procell Intense - Battery Group'] = test_url
    links_df['CR2032']['Procell Intense - Granada Batteries'] = test_url
    links_df['CR2032']['Procell Intense - Buy A Battery'] = test_url
    links_df['CR2032']['Procell Constant - Battery Station'] = test_url
    links_df['CR2032']['Procell Constant - Battery Group'] = test_url
    links_df['CR2032']['Procell Constant - Granada Batteries'] = test_url
    links_df['CR2032']['Procell Constant - Buy A Battery'] = test_url
    links_df['CR2032']['Energizer Industrial - Battery Station'] = test_url
    links_df['CR2032']['Energizer Industrial - Battery Group'] = test_url
    links_df['CR2032']['Energizer Industrial - Granada Batteries'] = test_url
    links_df['CR2032']['Energizer Industrial - Buy A Battery'] = test_url
    links_df['CR2032']['Varta Industrial - Battery Station'] = test_url
    links_df['CR2032']['Varta Industrial - Battery Group'] = test_url
    links_df['CR2032']['Varta Industrial - Granada Batteries'] = test_url
    links_df['CR2032']['Varta Industrial - Buy A Battery'] = test_url

    print(links_df)

    prices_df = pd.DataFrame(columns=battery_types, index=manufacturers_and_suppliers) #Make dataframe to store values in

    progress_counter = 0
    total_entries = len(manufacturers_and_suppliers) * len(battery_types)

    for battery_type in battery_types: #Loops through all combinations grabbing the prices from each page
        for storepage in manufacturers_and_suppliers:
            print('\r' + create_progress_bar(100 * (progress_counter / total_entries)), end='', flush=True)
            progress_counter += 1

            prices_df[battery_type][storepage] = get_battery_price(get_soup_from_url(links_df[battery_type][storepage]))

    print("\n\n\n\n")
    print(prices_df)

