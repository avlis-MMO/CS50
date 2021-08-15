import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.route("/", methods=["GET", "POST"])
def index():
    
    if request.method == "POST":
        
        name = request.form.get("name")
        day = request.form.get("day")
        month = request.form.get("month")
        print(name, day, month)
        if not name or not day or not month:
            
            return render_template("failure.html")
             
        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?,?,?)", name, month, day)
        
        return redirect("/")

    else:

        # TODO: Display the entries in the database on index.html
        birthdays = db.execute("SELECT * FROM birthdays")
        
        return render_template("index.html", birthdays = birthdays)

@app.route("/delete", methods=["POST"])
def delete():
    
    name = request.form.get("name")
    print(name)
    if name:
        db.execute("DELETE FROM birthdays WHERE name = ?", name)
    return redirect("/")
