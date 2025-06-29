#pragma once

/**
 * Model class for managing application data and logic.
 */
class Model {
public:
    /**
     * Constructor for Model class.
     */
    Model(void);

    /**
     * @brief Set the isometrical angle of the model.
     *
     * @param p_v The value to add to the isometric alpha.
     */
    void addIsoAlpha(float p_v);

    /**
     * @brief Add a rotation to the model.
     *
     * @param p_v The value to add to the rotation.
     */
    void addRotation(float p_v);

    /**
     * @brief Add a grid size to the model.
     *
     * @param p_v The value to add to the grid size.
     */
    void addGridSize(int p_v);

    /**
     * @brief Get the isometric alpha of the model.
     *
     * @return The current isometric alpha.
     */
    float getIsoAlpha(void) const;

    /**
     * @brief Get the rotation of the model.
     *
     * @return The current rotation.
     */
    float getRotation(void) const;

    /**
     * @brief Get the Grid Size object
     *
     * @return int The current grid size
     */
    int getGridSize(void) const;
private:
    float _isoAlpha;
    float _rotation;
    int _gridSize;
};