-- Keep a log of any SQL queries you execute as you solve the mystery.

-- get crime reports from relevant streets
SELECT street, description
FROM crime_scene_reports
WHERE street like "%humphrey%"

-- I notice their are two street named humphrey : humphrey street and humprehy lane
-- the crime took place on humphrey street, so adjust search accordingly
SELECT street, description, day
FROM crime_scene_reports
WHERE street = "Humphrey Street"
order by day

-- 7 reports from "Humphrey Street", only one on the day in question, the 28th
-- According to the report;
--      Duck was stolen at 10:15 From the bakery
--      Three witnesses were present at the time
-- next step is to check the interview logs from the day

SELECT id, name, transcript
FROM interviews
WHERE day = 28 AND month = 7

-- from transcript 161 by Ruth
--      Thief left within 10 mins of the theft by car
-- from transcript 162 by Eugene
--      thief withdrew money ealier in the day from atm on Leggett street
-- from transcript 163 by Raymond
--      thief called someone for less than 1 minute
--      thief said they would take earliest flight, the next day (29)
--      thief asked accomplice on phone to buy ticket

-- Bakery security logs:
SELECT hour, minute, activity, license_plate
from bakery_security_logs
where month = 7
and day = 28
and hour = 10
and minute between 15 and 25

-- 8 cars exited the parking lot in the 10 mins between 10:15 and 10:25
-- combine the liscense plate data from last query to query in people table

select id, name, phone_number, passport_number, license_plate
    from people
    where license_plate
    in (SELECT license_plate
            from bakery_security_logs
            where month = 7
            and day = 28
            and hour = 10
            and minute between 15 and 25)

-- returns a list of 8 suspects:
-- Vanessa, Barry, Iman, Sofia, Luca, Diana, Kelsey, Bruce

-- next step is to check which people withdrew money from legget street
select people.id, people.name, people.phone_number, people.passport_number
    from people
    join bank_accounts
        on people.id = bank_accounts.person_id
    join atm_transactions
        on bank_accounts.account_number = atm_transactions.account_number
    where atm_transactions.month = 7
        and atm_transactions.day = 28
        and atm_transactions.atm_location = "Leggett Street"
        and atm_transactions.transaction_type = "withdraw"
-- returns a list of 8 people who withdrew money from leggett street on the day

-- combing the two last queries
select people.id, people.name, people.phone_number, people.passport_number
    from people
    join bank_accounts
        on people.id = bank_accounts.person_id
    join atm_transactions
        on bank_accounts.account_number = atm_transactions.account_number
    where atm_transactions.month = 7
        and atm_transactions.day = 28
        and atm_transactions.atm_location = "Leggett Street"
        and atm_transactions.transaction_type = "withdraw"
        and people.name
            in (select name
                    from people
                    where license_plate
                    in (SELECT license_plate
                            from bakery_security_logs
                            where month = 7
                            and day = 28
                            and hour = 10
                            and minute between 15 and 25))

-- return a list of 4 suspects who both exited the bakery
-- between 10:15 and 10:25, and withdrew money on leggett street
-- on the day in question.
-- out suspects are Bruce, Diana, Iman, Luca

-- next lets use passengers, flight and airport table to find out who flew away and to where
select
    airports.city,
    people.name,
    flights.hour,
    flights.minute
    from airports
    join flights
        on airports.id = flights.destination_airport_id
    join passengers
        on flights.id = passengers.flight_id
    join people
        on passengers.passport_number = people.passport_number
        where flights.month = 7
            and flights.day = 29
            and flights.origin_airport_id = 8
            and people.name in ("Bruce", "Diana", "Iman", "Luca")

-- narrows our suspects down to 2: Bruce and Luca
-- who both withdrew money on leggett street, exited the bakery at the right time
-- and boarded an early flight out of town the next day to New York City

select name, phone_number
from people
where name in ("Bruce", "Luca")

-- returns name and phone number of suspects
-- lets compare to phone call to find out who made the call

select
    people.name,
    phone_calls.id,
    phone_calls.caller,
    phone_calls.receiver,
    phone_calls.duration
    from phone_calls
    join people
        on phone_calls.caller = people.phone_number
    where phone_calls.month = 7
        and phone_calls.day = 28
        and phone_calls.duration <= 60
        and phone_calls.caller in (
                select phone_number
                from people
                where name in ("Bruce", "Luca"))

-- narrows the list down to 1 person who additionally made a call less than 60 seconds long
-- on the day of the theft.
-- Bruce is the Theif who stole the duck.
-- the reciver of the call ((375) 555-8161) is the accomplice.

select name
from people
where phone_number = "(375) 555-8161"
-- returns the name of the accomplice: Robin.

-- Summary Bruce Stole the duck, with assistance of Robin, and Flew to New York City to escape justice.
