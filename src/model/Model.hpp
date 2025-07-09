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
     * @param updateIsoAlpha The value to add to the isometric alpha. (between -pi/2 and pi/2)
     */
    void addIsoAlpha(float updateIsoAlpha);

    /**
     * @brief Add a rotation to the model.
     *
     * @param updateRotationAngle The value to add to the rotation. (between -2pi and 2pi)
     */
    void addRotation(float updateRotationAngle);

    /**
     * @brief Add a grid size to the model.
     *
     * @param updateGridSize The value to add to the grid size. (beetween -5 and 5)
     */
    void addGridSize(int updateGridSize);

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
    float _isoAlphaAngle;
    float _rotationAngle;
    int _gridSize;
};