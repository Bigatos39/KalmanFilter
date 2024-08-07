# Kalman Filter
## Kalman Gain Calculation:

The Kalman Gain ($KG$) is computed as follows:

$KG = \dfrac{E_{EST}}{E_{EST} + E_{MEA}}$

Where:
- $E_{EST}$ is the prediction error.
- $E_{MEA}$ is the measurement error.

The resulting $KG$ value satisfies $0 \leq KG \leq 1$.

The update equation for estimating the current value ($EST_t$) is:

$EST_t = EST_{t - 1} + KG \left[MEA - EST_{t - 1}\right]$

The Kalman Gain helps stabilize the difference between predicted and measured values in estimating new states from previous ones.

## Current Value Estimation with $k$ from $0 \longrightarrow n$:

### Preparation of Previous State Parameters:
- When there are no predictions yet, the previous state is initialized with an initial state vector ($r_0$) and an initial covariance matrix ($C_0$).
- After predictions and updates, state parameters are derived from the previous state updates ($r_{k - 1 | k - 1}, C_{k - 1 | k - 1}$).

### Prediction of New State:
Predictions for the next state are made using parameters of the previous state:
$r_{k | k - 1} = F_k \cdot r_{k - 1 | k - 1}$
and the predicted covariance matrix:
$C_{k | k - 1} = F_k \cdot C_{k - 1 | k - 1} \cdot F_k^T + Q_k$

### State Update:
- Calculation of the Kalman Gain matrix:
  
$K_k = \dfrac{C_{k | k - 1} \cdot H_k}{H_k \cdot C_{k | k - 1} \cdot H_k^T + V_k}$

Where:
- $H_k$ is the observation matrix.
- $V_k$ is the variance matrix.

- Update of state vector:
$r_{k | k} = r_{k | k - 1} \cdot K\left[y_k - H \cdot r_{k | k - 1}\right]$

- Update of covariance matrix:
$C_{k | k} = \left(I - K_k \cdot H_k\right) \cdot C_{k | k - 1}$
Where $I$ is the identity matrix.

### Algorithm Iteration:
- If $k < n$, the position $k \longrightarrow \left(k - 1\right)$ and updated values are used for the next computation.
- If $k = n$, the algorithm terminates.
