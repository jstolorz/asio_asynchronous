
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string>

boost::mutex global_stream_lock;

void worker_thread(boost::shared_ptr<boost::asio::io_service> service, int counter) {
    global_stream_lock.lock();
    std::cout << "Thread " << counter << " Start.\n";
    global_stream_lock.unlock();

    while (true) {
        try {
            boost::system::error_code error;
            service->run(error);

            if (error) {
                global_stream_lock.lock();
                std::cout << "Error Code " << error << "\n";
                global_stream_lock.unlock();
            }

            break;
        } catch (std::exception &ex) {
            global_stream_lock.lock();
            std::cout << "Exception " << ex.what() << "\n";
            global_stream_lock.unlock();
        }

        global_stream_lock.lock();
        std::cout << "Thread " << counter << " End.\n";
        global_stream_lock.unlock();
    }
}

void on_accept(const boost::system::error_code &error) {
    if (error) {
        global_stream_lock.lock();
        std::cout << "On Accept Error " << error << "End.\n";
        global_stream_lock.unlock();
    } else {
        global_stream_lock.lock();
        std::cout << "Accepted! \n";
        global_stream_lock.unlock();
    }
}

int main() {

    boost::shared_ptr<boost::asio::io_service> service(
            new boost::asio::io_service
    );

    boost::shared_ptr<boost::asio::io_service::work> worker(
            new boost::asio::io_service::work(*service)
    );

    boost::shared_ptr<boost::asio::io_service::strand> strand(
            new boost::asio::io_service::strand(*service)
    );

    boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor(
            new boost::asio::ip::tcp::acceptor(*service)
    );

    boost::shared_ptr<boost::asio::ip::tcp::socket> socket(
            new boost::asio::ip::tcp::socket(*service)
    );

    global_stream_lock.lock();
    std::cout << "Press ENTER to exit!\n";
    global_stream_lock.unlock();

    boost::thread_group threads;
    for (int l = 0; l <= 2; ++l) {
        threads.create_thread(boost::bind(&worker_thread, service,l));
    }

    try{
        boost::asio::ip::tcp::resolver resolver(*service);
        boost::asio::ip::tcp::resolver::query query("127.0.0.1",boost::lexical_cast<std::string>(9999));
        boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

        acceptor->open(endpoint.protocol());
        acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(false));
        acceptor->bind(endpoint);
        acceptor->listen(boost::asio::socket_base::max_connections);
        acceptor->async_accept(*socket, boost::bind(&on_accept,_1));

        global_stream_lock.lock();
        std::cout << "Listening on: " << endpoint << std::endl;
        global_stream_lock.unlock();


    }catch (std::exception& ex){
        global_stream_lock.lock();
        std::cout << "Exception: " << ex.what() << ".\n";
        global_stream_lock.unlock();
    }

    std::cin.get();

    boost::system::error_code error;

    acceptor->close(error);

    socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
    socket->close();

    service->stop();

    threads.join_all();

    return 0;
}

