
#ifndef ABSTRACTOBJECTACTION_H
#define ABSTRACTOBJECTACTION_H

#include <qobject.h>

/* 
   Action class is used to be saved in Undo/Redo stack.
   Every action can be executed or unexecuted (rollback) 
   with following virtual methods. Use inheritance to 
   provide all the functionality.
*/
class AbstractObjectAction : public QObject
{
    Q_OBJECT
public:
    virtual void execute () = 0;
    virtual void unexecute () = 0;

signals:
    void actionExecuted ( AbstractObjectAction& );
    void actionUnexecuted ( AbstractObjectAction& );
};


/* 
   Just a handly template class, wich is used to resolve 
   common problems: keeps pointers to execute or unexecute
   methods, its params and so on.
*/ 
template <
          class Executor, 
          class ExecuteParams,
          class UnexecuteParams = ExecuteParams
         >
class ConcreteObjectAction : public AbstractObjectAction
{
    typedef void (Executor::* Execute)( ExecuteParams );
    typedef void (Executor::* Unexecute)( UnexecuteParams );

public:
    ConcreteObjectAction ( Executor& executor_,
                           Execute exec_, 
                           Unexecute unexec_,
                           ExecuteParams executeParams_,
                           UnexecuteParams unexecuteParams_ ) :
        executor(executor_),
        exec(exec_),
        unexec(unexec_),
        executeParams(executeParams_),
        unexecuteParams(unexecuteParams_)        
    {}

    void execute ()
    {        
       (executor.*exec)(executeParams);
        emit actionExecuted(*this);
    }

    void unexecute ()
    {
       (executor.*unexec)(unexecuteParams);
        emit actionUnexecuted(*this);
    }

private:
    Executor& executor;
    Execute exec;
    Unexecute unexec;
    ExecuteParams executeParams;
    UnexecuteParams unexecuteParams;
};

#endif //ABSTRACTOBJECTACTION_H
