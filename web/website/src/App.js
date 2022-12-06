import './App.css';
import Header from "./components/Header/Header";
import Home from "./components/Home/Home";
import Upload from './components/Upload/Upload';
import Team from './components/Team/Team';
import Footer from './components/Footer/Footer';

function App() {
  return (
    <div className="App">
      <Header />
      <Home />
      <Upload />
      <Team />
      <Footer />
    </div>
  );
}

export default App;
