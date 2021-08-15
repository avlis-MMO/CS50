from flask import Flask, render_template, request
from colour import Color
import requests
from bs4 import BeautifulSoup
import re
from math import log10, pow, log
app = Flask(__name__)


@app.route("/")
def index():

    # Load mindex when web is loaded
    return render_template("index.html")

@app.route("/CO2", methods = ["GET", "POST"])
def CO2():

    # Using beautiful soup to extract the information
    urll = "https://knoema.com/atlas/ranks/CO2-emissions?mode=amp"
    wb_site = requests.get(urll)
    soup = BeautifulSoup(wb_site.text, 'lxml')
    table_body = soup.find('tbody')
    table_head = soup.find('thead')
    rows_b = table_body.find_all('tr')
    rows_h = table_head.find_all('tr')
    countries = []
    emissions = []
    year_dic={}

    #Extract the country and CO2 emissions and use regex to select info
    for row in rows_b:
        head=row.find_all('th')
        cols=row.find_all('td')
        countries.append(re.findall(r'(?<=">)[A-Za-z].*(?=</a)', str(head).replace(" ","_").replace("/B","_B").replace("’","").replace("-","_")))
        emissions.append(re.findall(r'(?<=>)([0-9]?\w*[,]?[0-9]\w*[.]\w*)', str(cols).replace(",", "")))

    # Extract the years avaible and adding to a dict
    for row in rows_h:
        head=row.find_all('th')
        years=re.findall(r'(?<=>)[0-9]\w*', str(head))
    i=0
    for year in years:
        year_dic[year]=i
        i=i+1


    dic ={}
    color_dic ={}
    values=[0,1,5,10,50,100,500,1000,5000,10000]
    legend=['0 - 1', '1 - 5', '5 - 10', '10 - 50', '50 - 100', '100 - 500', '500 - 1 000', '1 000 - 5 000', '5 000 - 10 000', '> 10 000']

    # Creating color array to use in the egend
    white = Color("#ffdbdb")
    colors = list(white.range_to(Color("#470404"),len(legend)))

    # Load when a year is chosen
    if request.method=="POST":

        # Get the year they want to show
        year=request.form.get("year")
        i=0

        # Add the countries and corresponding emissions of the selected year to a dict
        for country in countries:
            dic[country[0]]=[float(emissions[i][year_dic[year]])]
            i=i+1

        # Matching the values of the emission to a color to make the colored map
        for i in range(0,len(legend)):
            for key in dic:
                if dic[key][0] > values[i]:
                    color_dic[key]=colors[i]

        return render_template("CO2.html", color_dic=color_dic, years=years, year=year, legends=zip(colors,legend), dic=dic)

    # Http request when page is first loaded
    else:

        # Add the countries and corresponding emissions of the selected year to a dict
        i=0
        for country in countries:
            n=len(emissions[i])
            dic[country[0]]=[float(emissions[i][n-1])]
            i=i+1

        # Matching the values of the emission to a color to make the colored map
        for i in range(0,len(legend)):
            for key in dic:
                if dic[key][0] > values[i]:
                    color_dic[key]=colors[i]

        # When first loaded load the most recent year as default
        return render_template("CO2.html", color_dic=color_dic, years=years, year=years[len(years)-1], legends=zip(colors,legend), dic=dic)

@app.route("/Pop", methods=["GET", "POST"])
def Pop():
    urll = "https://knoema.com/atlas/ranks/Population?mode=amp"
    wb_site = requests.get(urll)
    soup = BeautifulSoup(wb_site.text, 'lxml')
    table_body = soup.find('tbody')
    table_head = soup.find('thead')
    rows_b = table_body.find_all('tr')
    rows_h = table_head.find_all('tr')
    countries = []
    population = []
    year_dic={}

    #Extract the country and population and use regex to select info
    for row in rows_b:
        head=row.find_all('th')
        cols=row.find_all('td')
        countries.append(re.findall(r'(?<=">)[A-Za-z].*(?=</a)', str(head).replace(" ","_").replace("/B","_B").replace("'","").replace("-","_").replace(",","").replace(".","")))
        population.append(re.findall(r'(?<=>)[0-9]+', str(cols).replace(",", "")))

    # Extract the years avaible and adding to a dict
    for row in rows_h:
        head=row.find_all('th')
        years=re.findall(r'(?<=>)[0-9]\w*', str(head))
    i=0
    for year in years:
        year_dic[year]=i
        i=i+1

    dic ={}
    color_dic ={}
    values=[0,10000,50000,100000,1000000,10000000,50000000,100000000,500000000,1000000000]
    legend=['<10 000', '10 000 - 50 000', '50 000 - 100 000','100 000 - 1 000 000',
            '1 000 000 - 10 000 000', '10 000 000 - 50 000 000', '50 000 000 - 100 000 000',
            '100 000 000 - 500 000 000', '500 000 000 - 1 000 000 000', '>1 000 000 000']

    # Creating color array to use in the egend
    white = Color("#bad1f7")
    colors = list(white.range_to(Color("#053687"),len(legend)))

    # Load when a year is chosen
    if request.method=="POST":

        # Get the year they want to show
        year=request.form.get("year")
        i=0

        # Add the countries and corresponding population of the selected year to a dict
        # Make sure if any information of any country is missing to delete from the dict
        for country in countries:
            if country[0] == 'West_Bank_and_Gaza':
                population[i].insert(0, 0)
            if int(year) <= 2011:
                dic[country[0]]=[int(population[i][year_dic[year]])]
                i=i+1
            else:
                if country[0] == "Eritrea":
                    dic["Eritrea"]=None
                else:
                    dic[country[0]]=[ float(population[i][year_dic[year]])]
                    i=i+1
        dic.pop('Eritrea')
        dic.pop('West_Bank_and_Gaza')

        # Matching the values of the population to a color to make the colored map
        for i in range(0,len(legend)):
            for key in dic:
                if dic[key][0] > values[i]:
                    color_dic[key]=colors[i]

        return render_template("Population.html", color_dic=color_dic, years=years, year=year,legends=zip(colors,legend), dic=dic)

    else:

         # Add the countries and corresponding population of the selected year to a dict
        # Make sure if any information of any country is missing to delete from the dict
        i=0
        for country in countries:
            if country[0] == 'West_Bank_and_Gaza':
                population[i].insert(0, None)
            if country[0] == "Eritrea":
                dic["Eritrea"]=None
            else:
                dic[country[0]]=[int(population[i][len(years)-1])]
                i=i+1
        dic.pop('Eritrea')

        # Matching the values of the population to a color to make the colored map
        for i in range(0,len(legend)):
            for key in dic:
                if int(dic[key][0]) > values[i]:
                    color_dic[key]=colors[i]

        # When first loaded load the most recent year as default
        return render_template("Population.html", color_dic=color_dic, years=years, year=years[len(years)-1], legends=zip(colors,legend), dic=dic)
