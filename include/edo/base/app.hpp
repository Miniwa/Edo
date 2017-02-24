#ifndef EDO_APP_HPP
#define EDO_APP_HPP

namespace edo
{
    /**
     * A wrapper for the main loop of an application
     * Used by the deployment wrappers.
     */
    class IApplication
    {
    public:
        IApplication();

        /**
        * Subclasses will be deleted through references to this interface, abstract constructor needed
        */
        virtual ~IApplication() = 0;

        /**
         * Abstract main method of the application
         * Will be called by the run() method a given amount of times per second
         * Setting the exit flag causes the application to exit
         */
        virtual void main() = 0;

        /**
        * Runs the application until the exit flag is set
        * When this method returns, the application has finished
        */
        void run();
        
        /**
         * Returns whether the application should exit or not
         */
        bool shouldExit();

        /**
         * Sets the exit flag of the application to given value
         */
        void setExit(bool exit);

    private:
        bool exit;
    };
}
#endif