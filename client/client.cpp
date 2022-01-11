#include "../common/global.h"

boost::mutex global_stream_lock;

void worker_thread(boost::shared_ptr<boost::asio::io_service> service, int counter){

    global_stream_lock.lock();
    std::cout <<"Thread " << counter << " Start.\n";
    global_stream_lock.unlock();

    while (true){
        try {
            boost::system::error_code error;
            service->run(error);

            if(error){
                global_stream_lock.lock();
                std::cout <<"Error Code " << error << "\n";
                global_stream_lock.unlock();
            }

            break;
        }catch (std::exception& ex){
            global_stream_lock.lock();
            std::cout <<"Exception " << ex.what() << "\n";
            global_stream_lock.unlock();
        }
    }

    global_stream_lock.lock();
    std::cout <<"Thread " << counter << " End.\n";
    global_stream_lock.unlock();
}

void on_connect(const boost::system::error_code& error){
    if(error){
        global_stream_lock.lock();
        std::cout <<"Error Code " << error << "\n";
        global_stream_lock.unlock();
    }else{
        global_stream_lock.lock();
        std::cout <<"Connected! \n";
        global_stream_lock.unlock();
    }
}


int main(){

    boost::shared_ptr<boost::asio::io_service> service(
            new boost::asio::io_service
            );

    boost::shared_ptr<boost::asio::io_service::work> worker(
            new boost::asio::io_service::work(*service)
    );

    boost::shared_ptr<boost::asio::io_service::strand> strand(
            new boost::asio::io_service::strand(*service)
    );

    boost::shared_ptr<boost::asio::ip::tcp::socket> socket(
            new boost::asio::ip::tcp::socket(*service)
            );

    global_stream_lock.lock();
    std::cout <<"Press ENTER to exit! \n";
    global_stream_lock.unlock();

    boost::thread_group threads;
    for (int l = 0; l <= 2; ++l) {
        threads.create_thread(boost::bind(&worker_thread, service, l));
    }

    try {

        boost::asio::ip::tcp::resolver resolver(*service);
        boost::asio::ip::tcp::resolver::query query("127.0.0.1",boost::lexical_cast<std::string>(9999));
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
        boost::asio::ip::tcp::endpoint endpoint = *iterator;

        global_stream_lock.lock();
        std::cout <<"Connecting to: " << endpoint << "\n";
        global_stream_lock.unlock();

        boost::system::error_code ec;

        socket->async_connect(endpoint, boost::bind(&on_connect, ec));

    }catch (std::exception& ex){
        global_stream_lock.lock();
        std::cout <<"Exception " << ex.what() << "\n";
        global_stream_lock.unlock();
    }

    std::cin.get();

    boost::system::error_code ec;
    socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both,ec);
    socket->close();

    service->stop();

    threads.join_all();

    return 0;
}

