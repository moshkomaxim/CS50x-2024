-- Keep a log of any SQL queries you execute as you solve the mystery.
/*

-- Select crime by date
SELECT * FROM crime_scene_reports WHERE street = "Humphrey Street" AND year = 2021 AND month = 7 AND day = 28;

-- We get that crime took place at 10:15 AM at the bakery. We have some witnesses
SELECT * FROM interviews WHERE year = 2021 AND month = 7 AND day = 28;

-- Based on the information from intervies, we can find some details about license plate, phone number, and bank info
SELECT * FROM people WHERE license_plate IN (
SELECT license_plate FROM bakery_security_logs WHERE activity = "exit" AND year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 40
)
AND phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60
)
AND people.id IN (SELECT person_id FROM bank_accounts WHERE bank_accounts.account_number IN ( SELECT account_number FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"
)
);

-- Found information give us three suspect persons - Bruce, Diana and Taylor
SELECT *FROM people WHERE phone_number IN (SELECT receiver FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60 AND caller IN ("(286) 555-6063", "(770) 555-1861", "(367) 555-5533")
);

-- By the checking phone calls we get three related names - Robin, James, Philip

SELECT *FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60 AND caller IN ("(286) 555-6063", "(770) 555-1861", "(367) 555-5533")
AND receiver IN ("(676) 555-6554", "(725) 555-3243", "(375) 555-8161");

-- Knowing the names of the possible suspects and accomplices we can check who called who.
-- Using this, we can checked who called who
SELECT * FROM flights JOIN airports ON airports.id = flights.origin_airport_id
WHERE flights.year = 2021 AND flights.month = 7 AND flights.day = 29 AND flights.hour BETWEEN 00 AND 12 AND airports.city = "Fiftyville"
ORDER BY flights.hour, flights.minute;

SELECT full_name, city FROM airports WHERE id IN (1, 4, 11);

-- Thief said that he wanted to leave early tomorrow. By checking earliest flights, we get three flights
SELECT name, passport_number FROM people WHERE people.name IN ("Bruce", "Taylor", "Diana", "James", "Philip", "Robin")
AND passport_number IN (SELECT passport_number FROM passengers JOIN flights ON flights.id = passengers.flight_id
JOIN airports ON airports.id = flights.origin_airport_id WHERE flights.year = 2021 AND flights.month = 7 AND flights.day = 29 AND flights.hour BETWEEN 00 AND 12
AND flights.origin_airport_id IN (SELECT airports.id FROM airports WHERE airports.city = "Fiftyville") AND flights.destination_airport_id IN (SELECT airports.id
FROM airports WHERE airports.city IN ("New York City", "Chicago", "San Francisco")
)
);

-- Bruce and Taylor flew on the 29th on one of the three aforementioned flights.

SELECT full_name, city, passport_number, flights.destination_airport_id, flights.hour, flights.minute
FROM airports JOIN flights ON flights.origin_airport_id = airports.id JOIN passengers ON passengers.flight_id = flights.id
WHERE flights.year = 2021 AND flights.month = 7 AND flights.day = 29 AND hour BETWEEN 00 AND 12 AND passengers.passport_number IN (
SELECT passport_number FROM people WHERE people.name IN ("Bruce", "Taylor", "Diana", "James", "Philip", "Robin")
);

-- So finally we have only two options:
-- Thief: Taylor | Accomplice: James | Flew to: New York.
-- Thief: Bruce  | Accomplice: Robin | Flew to: New York.
