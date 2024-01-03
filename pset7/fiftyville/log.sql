-- Keep a log of any SQL queries you execute as you solve the mystery.
SELECT * FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28;
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time
-- – each of their interview transcripts mentions the bakery.

-- check interviews table for that day
SELECT * FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28;
-- -- those seem related:
-- | 161 | Ruth    | 2021 | 7     | 28  | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
-- | 162 | Eugene  | 2021 | 7     | 28  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
-- | 163 | Raymond | 2021 | 7     | 28  | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |

-- the thief there withdraw money
-- check atm transactions for the day on Leggett Street
-- get the person name from people table
SELECT people.name, atm.atm_location, atm.transaction_type, atm.amount FROM atm_transactions AS atm
JOIN bank_accounts ON atm.account_number = bank_accounts.account_number
JOIN people ON bank_accounts.person_id = people.id
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street";
-- +---------+----------------+------------------+--------+
-- |  name   |  atm_location  | transaction_type | amount |
-- +---------+----------------+------------------+--------+
-- | Bruce   | Leggett Street | withdraw         | 50     |
-- | Kaelyn  | Leggett Street | deposit          | 10     |
-- | Diana   | Leggett Street | withdraw         | 35     |
-- | Brooke  | Leggett Street | withdraw         | 80     |
-- | Kenny   | Leggett Street | withdraw         | 20     |
-- | Iman    | Leggett Street | withdraw         | 20     |
-- | Luca    | Leggett Street | withdraw         | 48     |
-- | Taylor  | Leggett Street | withdraw         | 60     |
-- | Benista | Leggett Street | withdraw         | 30     |
-- +---------+----------------+------------------+--------+

-- they called someone who talked to them for less than a minute
-- check phone calls from this day with duration < 60s
-- get the names from caller and receivers from people table
SELECT pc.id, caller_people.name AS caller_name, receiver_people.name AS receiver_name, pc.duration
FROM phone_calls pc
JOIN people caller_people ON pc.caller = caller_people.phone_number
JOIN people receiver_people ON pc.receiver = receiver_people.phone_number
WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;
-- +-----+-------------+---------------+----------+
-- | id  | caller_name | receiver_name | duration |
-- +-----+-------------+---------------+----------+
-- | 221 | Sofia       | Jack          | 51       |
-- | 224 | Kelsey      | Larry         | 36       |
-- | 233 | Bruce       | Robin         | 45       |
-- | 251 | Kelsey      | Melissa       | 50       |
-- | 254 | Taylor      | James         | 43       |
-- | 255 | Diana       | Philip        | 49       |
-- | 261 | Carina      | Jacqueline    | 38       |
-- | 279 | Kenny       | Doris         | 55       |
-- | 281 | Benista     | Anna          | 54       |
-- +-----+-------------+---------------+----------+


-- check security footage from the bakery parking lot around 10:15 AM
-- "Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away."
-- get the names for the licenses from people table
SELECT bl.hour, bl.minute, bl.activity, bl.license_plate, people.name  FROM bakery_security_logs AS bl
JOIN people ON bl.license_plate = people.license_plate
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25
-- +------+--------+----------+---------------+---------+
-- | hour | minute | activity | license_plate |  name   |
-- +------+--------+----------+---------------+---------+
-- | 10   | 16     | exit     | 5P2BI95       | Vanessa |
-- | 10   | 18     | exit     | 94KL13X       | Bruce   |
-- | 10   | 18     | exit     | 6P58WS2       | Barry   |
-- | 10   | 19     | exit     | 4328GD8       | Luca    |
-- | 10   | 20     | exit     | G412CB7       | Sofia   |
-- | 10   | 21     | exit     | L93JTIZ       | Iman    |
-- | 10   | 23     | exit     | 322W7JE       | Diana   |
-- | 10   | 23     | exit     | 0NTHK55       | Kelsey  |
-- +------+--------+----------+---------------+---------+


-- try to find the earliest flight out of Fiftyville tomorrow
SELECT f.id AS flight_id, origin.city AS origin_city, destination.city AS destination_city, f.hour,f.minute FROM flights f
JOIN airports origin ON f.origin_airport_id = origin.id
JOIN airports destination ON f.destination_airport_id = destination.id
WHERE f.year = 2021 AND f.month = 7 AND f.day = 29
ORDER BY f.hour ASC;
-- +-----------+-------------+------------------+------+--------+
-- | flight_id | origin_city | destination_city | hour | minute |
-- +-----------+-------------+------------------+------+--------+
-- | 36        | Fiftyville  | New York City    | 8    | 20     |
-- ...


-- look at the passenger list for this flight (id = 36)
-- match the passport_number to the person name
SELECT passengers.*, people.name FROM passengers
JOIN people ON passengers.passport_number = people.passport_number
WHERE flight_id = 36;
-- +-----------+-----------------+------+--------+
-- | flight_id | passport_number | seat |  name  |
-- +-----------+-----------------+------+--------+
-- | 36        | 7214083635      | 2A   | Doris  |
-- | 36        | 1695452385      | 3B   | Sofia  |
-- | 36        | 5773159633      | 4A   | Bruce  |
-- | 36        | 1540955065      | 5C   | Edward |
-- | 36        | 8294398571      | 6C   | Kelsey |
-- | 36        | 1988161715      | 6D   | Taylor |
-- | 36        | 9878712108      | 7A   | Kenny  |
-- | 36        | 8496433585      | 7B   | Luca   |
-- +-----------+-----------------+------+--------+

-- intersect between people who withdraw money and people on the flight the next day
SELECT people.name FROM atm_transactions AS atm
JOIN bank_accounts ON atm.account_number = bank_accounts.account_number
JOIN people ON bank_accounts.person_id = people.id
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street"
INTERSECT
SELECT people.name FROM passengers
JOIN people ON passengers.passport_number = people.passport_number
WHERE flight_id = 36;
-- +--------+
-- |  name  |
-- +--------+
-- | Bruce  |
-- | Kenny  |
-- | Luca   |
-- | Taylor |


-- By elimination I can tell that Bruce is the thief,
-- Besides being on the flight (to nyc) the next day and withdrawing money,
-- he was seen short after exiting the bakery
-- and he called Robin (his accomplice)
