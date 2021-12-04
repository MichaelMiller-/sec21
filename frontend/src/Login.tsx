import Button from "react-bootstrap/Button";
import {Container, Form, Modal, Row} from "react-bootstrap";
import React, {useState} from "react";

interface LoginState {
    email: string;
    name: string;
}

const Login = () => {

    const [data, setData] = useState<LoginState>({
        email: "",
        name: "",
    })

    const handleInputChange = (e: { target: { name: any; value: any; }; }) => {
        const {name, value} = e.target;
        setData({...data, [name]: value,});
    };

    const handleSubmit = async (event: { preventDefault: () => void; }) => {
        event.preventDefault()
    };

   return (
       <Container>
           <Row className="justify-content-md-center">
           <Form onSubmit={handleSubmit}>
               <Modal.Header>
                    Login
               </Modal.Header>
               <Modal.Body>
                   <Form.Group controlId="validationEmailAddress">
                       <Form.Label>eMail Address</Form.Label>
                       <Form.Control
                           required
                           type="text"
                           name="email"
                           placeholder="example@organization.org"
                           value={data.email}
                           onChange={handleInputChange}
                       />
                   </Form.Group>
                   <Form.Group controlId="validationPassword">
                       <Form.Label>Password</Form.Label>
                       <Form.Control type="password" placeholder="Password" />
                   </Form.Group>
               </Modal.Body>
               <Modal.Footer>
                   <Button type="submit">Login</Button>
               </Modal.Footer>
           </Form>
           </Row>
       </Container>
   )
}

export default Login;