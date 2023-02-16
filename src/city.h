/*
 * This file is part of TSP_SA.
 *
 * Copyright © 2023 Diego Sebastián Sánchez Correa
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

/**
 * Creates a new City.
 * @param id the id of the city.
 * @param x the x coordinate of the city.
 * @param y the y coordinate of the city.
 * @param country the country of the city.
 * @param name the name of the city.
 */
City* city_new(int id, int x, int y,
               char* country,
               char* name);

/**
 * Frees the memory used by the city.
 * @param city the city to be freed.
 */
void city_free(City* city);

/**
 * Returns the id of the city.
 * @param city the city.
 * @return the id of the city.
 */
int city_id(City* city);

/**
 * Returns the x coordinate of the city.
 * @param city the city.
 * @return the x coordinate of the city.
 */
int city_x_coordinate(City* city);

/**
 * Returns the y coordinate of the city.
 * @param city the city.
 * @return the y coordinate of the city.
 */
int city_y_coordinate(City* city);

/**
 * Returns the country of the city.
 * @param city the city.
 * @return the country of the city.
 */
char* city_country(City* city);

/**
 * Returns the name of the city.
 * @param city the city.
 * @return the name of the city.
 */
char* city_name(City* city);

/**
 * Computes the distance between two cities.
 * @param c the first city.
 * @param d the second city.
 * @retuen the distance.
 */
float city_distance(City* c, City* d);
