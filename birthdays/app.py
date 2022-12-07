import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    error = ""
    if request.method == "POST":
        name = request.form.get("name")
        day = request.form.get("day")
        month = request.form.get("month")
        year = request.form.get("year")
        # Validate & error handling
        if name == "":
            error = "please enter a name"
        elif day == "" or month == "" or year == "":
            error = "please enter a valid date"
        else:
            invalid = False
            try:
                day = int(day)
                month = int(month)
                year = int(year)
            except:
                invalid = True
            if invalid:
                error = "please enter a numerical date"
            elif day < 1 or day > 31:
                error = "day is not valid"
            elif month < 1 or month > 12:
                error = "month is not valid"
            elif year < 1900 or year > 2300:
                error = "year is not valid"
            else:
                insert_query = f"INSERT INTO birthdays (name, day, month, year) VALUES ('{name}',{day},{month},{year})"
                error = "Succesfully Added!"
                db.execute(insert_query)

    birthdays = db.execute("SELECT * FROM birthdays")
    return render_template("index.html", birthdays=birthdays, error = error)


