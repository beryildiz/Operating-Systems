#include <iostream>

#include "client.hpp"
#include "queue.hpp"

using std::unique_lock;
using std::mutex;
using std::cout;
using std::cerr;
using std::unique_lock;
using std::defer_lock;
using std::size_t;

Client::Client(Queue& fifo, WebRequest& req, int id) : m_fifo(fifo), m_req(req) ,m_id(id) {
    clientSpawn();
}

Client::~Client(){}

string Client::getDomain(string url) {
    size_t start, end;

    if ( (start = url.find("//", 0)) != string::npos ) {
        start += 2;
        if ( (end = url.find('/', start) ) == string::npos) {
            end = url.length();
        }
        if (end > start) {
            return url.substr(start, end-start);
        }
    }
    return "";
}

string Client::toString(stringstream& stream) {
    string out = stream.str();
    stream.str("");
    stream.clear();

    return out; 
}

void Client::client() {
    int count;
    string domain, url;
    stringstream stream;

    unique_lock<mutex> lck(m_fifo.mut, defer_lock);

	while(true) {
        lck.lock();
        // while -> spinlock
		if (m_fifo.isEmpty()) {
            cout << "client_" << m_id << ": queue EMPTY.\n";
            if (m_fifo.isFinished()) {
                cout << "client_" << m_id << " exited.\n";
                lck.unlock();
                return;
            }
			m_fifo.notEmpty.wait(lck);
		}
        url = m_fifo.delItem();
        count = m_fifo.getCount();
        cout << "client_" << m_id << ": read " << url << " from queue.\n";
		lck.unlock();
		m_fifo.notFull.notify_one();

        domain = this->getDomain(url);
        if (domain == "") {
            cerr << "[ERROR] domain could not be extracted from URL [" << url << "] skipping\n";
        } else {
            string filename;
            
            stream << m_id << "_" << count << "_" << domain << ".html";
            filename = toString(stream);

            cout << "[START] Downloading URL: " << url << " ->> File: " << filename << "\n";

            int res = 0;
#if USE_PROXY
            try {
                res = m_req.downloadViaProxy(url, filename);
            }
#else
            try {
                res = m_req.download(url, filename);  
            }
#endif
            catch (std::runtime_error& e) {
                cerr << "[ERROR] URL: " << url << ", Message: " << e.what() << "\n"; 
            }
            if (res != WEBREQ_HTTP_OK) {
                cerr << "[ERROR] HTTP Status " << res << " returned for URL: " << url << "\n";
            } else {
                cout << "[DONE ] URL: " << url << " ->> File: " << filename << "\n";
            }
        }
	}  
}

void Client::clientSpawn() {
    m_thread = thread( [this] { this->client(); } );
}