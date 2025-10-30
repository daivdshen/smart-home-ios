typedef std::function<bool (unsigned int)> PeriodicExecutionCb;

struct PeriodicExecutionEntry
{
    unsigned int        periodMs;
    unsigned int        timeRef;
    PeriodicExecutionCb callback;

    PeriodicExecutionEntry(unsigned int period, PeriodicExecutionCb cb):
        periodMs(period), timeRef(0), callback(cb) { }
};

typedef std::vector<PeriodicExecutionEntry> PeriodicExecutions;

/**
 * @brief The PeriodicExecution class implements a timer with
 *        multiple attached callbacks. Each callback execution time
 *        is computed in order to maintain a regular execution for all
 *        callbacks. There is a callback parameter, ticks, which informs
 *        the caller on how many periods expired since last call.
 */
class PeriodicExecution
{
public:

    PeriodicExecution();

    /**
     * @brief addPeriodicCallback add a new callback to be periodically called
     * @param periodMs period for the callback [ms]
     * @param periodicCall callback function
     *
     * @note minimum period must be 20+ ms (suggested: 100 ms)
     */
    void addPeriodicCallback(unsigned int periodMs, PeriodicExecutionCb &&periodicCall);

    /**
     * @brief setContinousCallback add a unique special callback to be called
     *        as soon as possible (at least 10 ms between calls)
     * @param continousCall callback function
     */
    void setContinousCallback(PeriodicExecutionCb &&continousCall);

    /**
     * @brief start start the timer (blocking call)
     * @return false, if already started; otherwise true
     */
    bool start();

    /**
     * @brief stop stop the timer in execution
     * @return false, if already stopped; otherwise true
     */
    bool stop();

private:

    bool                started;
    PeriodicExecutions  periodicCbs;
    PeriodicExecutionCb continousCb;
};
