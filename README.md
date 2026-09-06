# Predicting Dissolved Oxygen Dynamics in an Argon Plasma Jet via Optical Emission Spectroscopy and Machine Learning

This repository implements a hierarchical machine learning framework for predicting dissolved oxygen (DO) behavior from optical emission spectroscopy (OES) data of an argon atmospheric pressure plasma jet.

## Project Goal

Raw spectral data is extremely high-dimensional (thousands of wavelength variables against a limited number of experimental samples). This project covers the full pipeline from raw data to a deployable model:

- Hierarchical, block-wise spectral feature selection and dimensionality reduction
- Training and comparison of tree-based regression models (XGBoost and Random Forest)
- Hyperparameter optimization via randomized and grid search
- Robust model evaluation using repeated cross-validation and an independent hold-out test set
- Interpretability of the final model via SHAP analysis

## Repository Structure

```
.
├── read data-normaling.ipynb        # Loading, cleaning, and normalizing raw spectral data
├── main.ipynb                       # Main hierarchical feature-selection pipeline
├── XGBoostVSrandonForest.ipynb      # Initial comparison of XGBoost and Random Forest
├── XGBoostGrdSearchCV.ipynb         # Final hyperparameter refinement for XGBoost
├── plotshap.ipynb                   # Model interpretability analysis with SHAP
├── predicted_vs_actual.png          # Predicted vs. actual values plot
├── saved_models/                    # Model, scaler, and feature names for the original version
├── saved_modelsNestedCV/            # Saved pipeline for nested cross-validation evaluation
├── XGBoostModel23/                  # Saved XGBoost model and its associated scaler
├── XGBOOSTGridSearchCV/             # Final GridSearchCV-tuned model and its scaler
├── RandomForestModel2/              # Random Forest baseline model and its scaler
└── requirements.txt
```

## Results Summary

| Model | Tuning Method | R² (Train) | R² (Test) | RMSE (Test) | CV R² (Mean ± SD) |
|---|---|---|---|---|---|
| XGBoost (Initial) | RandomizedSearchCV (15 folds) | 0.9496 | 0.6753 | 0.0539 | 0.3991 ± 0.0775 |
| Random Forest | RandomizedSearchCV (15 folds) | 0.8500 | 0.6690 | 0.0544 | 0.3802 ± 0.0881 |
| XGBoost (Final) | GridSearchCV (6 folds) | 0.9646 | 0.7412 | 0.0481 | 0.3889 ± 0.0852 |

The final GridSearchCV-refined XGBoost model was selected for deployment based on its simultaneous improvement in hold-out test accuracy and cross-validation stability. All data-dependent steps (feature selection, scaling, and hyperparameter tuning) were performed exclusively on the training data, with the independent test set used only for final evaluation.

## Installation

```bash
pip install -r requirements.txt
```

Core dependencies: `numpy`, `pandas`, `scikit-learn`, `xgboost`, `shap`, `joblib`.

## Usage

The notebooks are designed to be run in the following order, from raw data to final model:

1. `read data-normaling.ipynb` — Prepare and normalize the input data
2. `main.ipynb` — Run the hierarchical feature-selection pipeline and build the final feature set
3. `XGBoostVSrandonForest.ipynb` — Train and compare baseline models
4. `XGBoostGrdSearchCV.ipynb` — Final hyperparameter refinement of the selected model
5. `plotshap.ipynb` — Feature importance and interpretability analysis

Trained models and scalers are saved under the `saved_models*` directories and the model-specific folders, and can be loaded to run predictions on new experimental data.

## Citation

This code accompanies the manuscript "A hierarchical machine learning framework for predicting dissolved oxygen dynamics from optical emission spectra of an argon atmospheric pressure plasma jet." Please cite the paper if you use this repository.
