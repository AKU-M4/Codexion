*This project has been created as part of the 42 curriculum by adkaid-s.*

# Codexion

## Description
Codexion is a multithreaded concurrency simulation developed in C using POSIX threads (`pthreads`). It models the classic Dining Philosophers problem in a modern software engineering context: multiple "coders" sit at a circular table sharing USB hardware "dongles" required to compile quantum code.

Each coder continually cycles through:
1. **Acquiring Dongles**: A coder must hold two adjacent dongles (left and right) simultaneously to compile.
2. **Compiling**: The coder holds both dongles for `time_to_compile` milliseconds.
3. **Releasing & Cooldown**: Both dongles are released back to the table, entering a mandatory `dongle_cooldown` period before they can be acquired again.
4. **Debugging**: The coder spends `time_to_debug` milliseconds debugging without dongles.
5. **Refactoring**: The coder spends `time_to_refactor` milliseconds refactoring, after which they immediately attempt to acquire dongles and compile again.

If any coder fails to start compiling within `time_to_burnout` milliseconds from the start of the simulation or from their last compilation start, they experience a **burnout**, and the simulation halts.

## Instructions

### Compilation
Compile the project using `make`:
```bash
make
```
This produces the `codexion` executable with `-Wall -Wextra -Werror -pthread`.

Other standard Makefile rules:
```bash
make clean    # Remove object files
make fclean   # Remove object files and executable
make re       # Rebuild the project from scratch
```

### Execution
Run the simulation with the 8 mandatory arguments:
```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

#### Arguments
- `number_of_coders`: Number of coders and shared dongles ($N \ge 1$).
- `time_to_burnout`: Time (ms) without compiling before a coder burns out ($> 0$).
- `time_to_compile`: Time (ms) required to compile ($> 0$).
- `time_to_debug`: Time (ms) spent debugging ($> 0$).
- `time_to_refactor`: Time (ms) spent refactoring ($> 0$).
- `number_of_compiles_required`: Target compile count per coder before simulation terminates ($> 0$).
- `dongle_cooldown`: Cooldown time (ms) after dongle release before it can be retaken ($\ge 0$).
- `scheduler`: Dongle arbitration policy: `fifo` or `edf`.

#### Examples
- **Feasible run (EDF, 4 coders)**:
  ```bash
  ./codexion 4 1000 200 200 200 4 200 edf
  ```
- **Feasible run (EDF, 5 coders)**:
  ```bash
  ./codexion 5 1500 200 200 200 4 200 edf
  ```
- **Burnout detection test**:
  ```bash
  ./codexion 5 1000 200 200 200 4 200 edf
  ```

## Resources
- **POSIX Threads (IEEE Std 1003.1c)**: Documentation on mutexes (`pthread_mutex_t`), condition variables (`pthread_cond_t`), and thread lifecycle management.
- **Dining Philosophers Problem & Coffman Conditions**: Classic synchronization literature on deadlock and resource allocation graphs (E.W. Dijkstra, 1965; E.G. Coffman, 1971).
- **Earliest Deadline First (EDF) Scheduling**: Real-time scheduling theory for preemptive and non-preemptive task sets (Liu and Layland, 1973).
- **AI Usage**: AI assistance was utilized to perform static analysis of race conditions, audit heap comparator edge cases, inspect logs for thread desynchronization, and draft architectural documentation.

## Blocking Cases Handled
- **Deadlock Prevention & Coffman's Conditions**:
  - *Circular Wait*: Broken by enforcing a strict global resource hierarchy: dongles are always locked in ascending order of their IDs (`first = min(left, right)`, `second = max(left, right)`).
  - *Hold and Wait*: Eliminated by requiring atomic acquisition in `acquire_both`. A coder only takes possession of dongles when both are ready and arbitration is granted, preventing idle resource hoarding.
- **Starvation Prevention & Liveness**:
  - Under `edf` scheduling, each dongle maintains a min-heap priority queue ordered by deadline (`last_compile_time + time_to_burnout`). The coder closest to burnout is granted priority, ensuring liveness across all feasible parameter configurations.
- **Dongle Cooldown Handling**:
  - Dongles track an absolute timestamp `available_at`. Threads waiting on a dongle condition variable timedwait precisely until cooldown expiration or 5 ms slices, avoiding busy waiting and minimizing scheduling latency.
- **Precise Burnout Detection**:
  - A dedicated monitor thread polls coder compile timestamps every 500 microseconds, guaranteeing that any burnout is detected and announced well within the 10 ms specification window.
- **Log Serialization & Post-Burnout Clean Halt**:
  - Output is strictly serialized via `sim->log_lock`. When `burned out` is logged, `sim->stop` is set to 1 atomically under the log lock, and all condition variables are broadcast. Subsequent log attempts immediately abort, ensuring no state changes are printed after simulation termination.

## Thread Synchronization Mechanisms
- `pthread_mutex_t`:
  - `dongle->lock`: Protects individual dongle state (`in_use`, `available_at`, and the `waiters` heap).
  - `coder->state_lock`: Protects coder-specific variables (`last_compile_time`, `compiles_done`).
  - `sim->log_lock`: Serializes stdout and guards against post-burnout log emissions.
  - `sim->stop_lock`: Guards the global `stop` flag.
  - `sim->start_lock`: Functions as a synchronization barrier at launch, ensuring all coder threads and the monitor start simultaneously with zero skew.
- `pthread_cond_t`:
  - `dongle->cond`: Used to suspend coders waiting for a dongle to be released or cooldown to expire, avoiding CPU-spinning while maintaining instant wakeups on release or shutdown.
