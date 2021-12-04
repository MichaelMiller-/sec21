import {BrowserRouter, Route, Switch} from 'react-router-dom';
import {Container} from "react-bootstrap";
import React from "react";
import ProjectList from "./ProjectList";
import {Project, RouterComponentProjectProps} from "./Project";
import Login from "./Login";

const App = () => {

    console.log("backend: ", process.env.REACT_APP_BACKEND);
    console.log("services: ", process.env.REACT_APP_SERVICE_PROVIDER);
    console.log("download URL: ", process.env.REACT_APP_DOWNLOAD_URL);

    return (
        <Container fluid>
            <BrowserRouter>
                <Switch>
                    <Route exact path="/404">not found</Route>
                    <Route exact path="/"><ProjectList/></Route>
                    <Route
                        exact
                        path="/project/:projectId"
                        render={({match}: RouterComponentProjectProps) => (
                            <Project projectId={match.params.projectId}/>)}
                    />
                    <Route exact path="/login"><Login/></Route>
                </Switch>
            </BrowserRouter>
        </Container>
    );
};

export default App;
