-- Keep a log of any SQL queries you execute as you solve the mystery.

-- See what happened at the crime scene on that day and if there is extra information.
SELECT description FROM crime_scene_reports WHERE day = 28 AND month = 7 AND year = 2020 AND street = 'Chamberlin Street';

--RELEVANT INFORMATION: Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. Interviews were conducted today with 
--three witnesses who were present at the time each of their interview transcripts mentions the courthouse.

--Need to find what the witnesses saw and their names that mention the courthouse
SELECT name, transcript FROM interviews WHERE day >= 28 AND month = 7 AND year = 2020 AND transcript LIKE '%courthouse%';

--RELEVANT INFORMATION: name | transcript
                    --Ruth | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away. If you have 
                    --security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.
                    
                    --Eugene | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse,
                    --I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
                    
                    --Raymond | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In the call, 
                    --I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the 
                    --person on the other end of the phone to purchase the flight ticket.
                    
--Check the license plate of the car that exit the courthouse within 10 minutes after the incident 10:15 am - 10:25 am
SELECT hour, minute, license_plate FROM courthouse_security_logs WHERE activity = 'exit' AND day = 28 AND month = 7 AND year = 2020 AND hour = 10 AND minute >= 15 AND minute <= 25 ORDER BY minute ASC;

-- RELEVANT INFORMATION: time | license plate
                    --10:16 | 5P2BI95 
                    --10:18 | 94KL13X 
                    --10:18 | 6P58WS2 
                    --10:19 | 4328GD8 
                    --10:20 | G412CB7 
                    --10:21 | L93JTIZ 
                    --10:23 | 322W7JE
                    --10:23 | 0NTHK55
                    
--Check the ATM transactions at Fifer Street on the day of the crime 
SELECT account_number, amount FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2020 AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw';

-- RELEVANT INFORMATION: account number | amount
                    --28500762 | 48
                    --28296815 | 20
                    --76054385 | 60
                    --49610011 | 50
                    --16153065 | 80
                    --25506511 | 20
                    --81061156 | 30
                    --26013199 | 35

--Check the call logs the lasted less than a minute
SELECT caller, receiver FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2020 AND duration < 60;

--RELEVANT INFORMATION: caller | receiver
                    --(130) 555-0289 | (996) 555-8899
                    --(499) 555-9472 | (892) 555-8872
                    --(367) 555-5533 | (375) 555-8161
                    --(499) 555-9472 | (717) 555-1342
                    --(286) 555-6063 | (676) 555-6554
                    --(770) 555-1861 | (725) 555-3243
                    --(031) 555-6622 | (910) 555-3251
                    --(826) 555-1652 | (066) 555-9701
                    --(338) 555-6650 | (704) 555-2131


--Now check the people who own those acount numbers, have one of those phone numbers and license plate
SELECT name FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE account_number IN
(SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2020 AND atm_location = 'Fifer Street' AND transaction_type = 'withdraw')
AND license_plate IN
(SELECT license_plate FROM courthouse_security_logs WHERE activity = 'exit' AND day = 28 AND month = 7 AND year = 2020 AND hour = 10 AND minute >= 15 AND minute <= 25)
AND phone_number IN
(SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2020 AND duration < 60);

--RELEVANT INFORMATION: name
                    --Ernest
                    --Russel

--Check the name and abreviation of the airport he left

SELECT abbreviation, full_name FROM airports WHERE city = 'Fiftyville';

--RELEVANT INFORMATION: abbreviation | full_name
                    --CSF | Fiftyville Regional Airport
                    
--Check the flights from Fiftyville Regional Airport and see the earliest on the 29/07/2020 and the destanation 
--RELEVANT INFORMATION: hour | minute | abbreviation | full_name | city
                    --8 | 20 | LHR | Heathrow Airport | London
                    --9 | 30 | ORD | O'Hare International Airport | Chicago
                    --12 | 15 | SFO | San Francisco International Airport | San Francisco
                    --15 | 20 | HND | Tokyo International Airport | Tokyo
                    --16 | 0 | BOS | Logan International Airport | Boston
                    
--The earliest is the flight to London, now we need to check the people on the flight and see wich of our main suspects was there

SELECT name FROM people WHERE passport_number IN
(SELECT passport_number FROM passengers WHERE flight_id = (SELECT id FROM flights WHERE year = 2020 AND month = 7 AND day = 29 AND hour = 8 AND minute = 20));

--RELEVANT INFORMATION: name
                    --Bobby
                    --Roger
                    --Madison
                    --Danielle
                    --Evelyn
                    --Edward
                    --Ernest
                    --Doris
                    
--Wee see that Ernest was on the flight he is the thief
--Now we just need to see who he called at that time

SELECT phone_number FROM people WHERE name = 'Ernest';

--RELEVANT INFORMATION: phone number
                    --(367) 555-5533
--From previous information we sse that he called (375) 555-8161 which belongs to:
SELECT name FROM people WHERE phone_number = '(375) 555-8161';

--RELEVANT INFORMATION: name
                    --Berthold

--He is the accomplice




